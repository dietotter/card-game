#include "Server.h"
#include "events/ConnectionEvent.h"

#include <memory>
#include <stdexcept>
#include <iostream>

namespace nik {

    std::mutex Server::g_inMutex;
    std::mutex Server::g_outMutex;

    std::thread Server::m_serverInThread{};
    std::thread Server::m_serverOutThread{};
    sf::TcpListener Server::m_listener{};
    std::map<int, std::unique_ptr<sf::TcpSocket>> Server::m_clients{};
    Server::EventsQueue Server::m_incomingEvents{};
    Server::EventsQueue Server::m_outgoingEvents{};
    std::mutex Server::m_clientsMutex{};
    bool Server::m_running{};

    void Server::listen(int port)
    {
        if (isRunning())
        {
            throw std::runtime_error("Server is already running on port " + std::to_string(port));
        }

        if (m_listener.listen(port) != sf::Socket::Done)
        {
            throw std::runtime_error("Server couldn't be created on port " + std::to_string(port));
        }

        m_running = true;
        m_serverInThread = std::thread{ inThreadUpdate };
        m_serverOutThread = std::thread{ outThreadUpdate };
    }

    void Server::stop()
    {
        m_running = false;

        if (m_serverInThread.joinable())
        {
            m_serverInThread.join();
        }

        // ? maybe this should be moved to the wrap up section on inThreadUpdate ?
        if (m_serverOutThread.joinable())
        {
            m_serverOutThread.join();
        }
    }

    void Server::inThreadUpdate()
    {
        sf::SocketSelector selector;
        selector.add(m_listener);

        static int clientCounter{ 0 };

        while (isRunning())
        {
            // selector.wait(1 sec) will be blocking for 1 second (waiting for events in listener and sockets), and
            // then will return false. This is needed for the while loop to run and check for if the server is still running.
            // selector.wait() would be blocking for infinity (until some event happens)
            if (selector.wait(sf::seconds(1.f)))
            {
                if (selector.isReady(m_listener))
                {
                    auto client{ std::make_unique<sf::TcpSocket>() };
                    if (m_listener.accept(*client) == sf::Socket::Done)
                    {
                        selector.add(*client);

                        m_clientsMutex.lock();
                        m_clients.insert({++clientCounter, std::move(client)});
                        m_clientsMutex.unlock();

                        // create event to notify all clients about the new connection
                        g_outMutex.lock();
                        m_outgoingEvents.push_back(std::make_unique<ClientConnectedEvent>(clientCounter, clientCounter));
                        g_outMutex.unlock();

                        // create event for server to handle new connection
                        g_inMutex.lock();
                        m_incomingEvents.push_back(std::make_unique<ClientConnectedEvent>(clientCounter));
                        g_inMutex.unlock();
                    }
                }
                else
                {
                    for (auto &clientPair : m_clients)
                    {
                        auto &client{ *clientPair.second };
                        if (selector.isReady(client))
                        {
                            sf::Packet packet;
                            if (client.receive(packet) == sf::Socket::Done)
                            {
                                sf::Packet packetCopy{ packet };
                                sf::Uint8 type;
                                packetCopy >> type;

                                std::lock_guard<std::mutex> guard(g_inMutex);
                                // create event depending on the event type, received from package
                                m_incomingEvents.push_back(NetworkEvent::createEventByType(type));
                                packet >> *m_incomingEvents.back();
                            }
                        }
                    }
                }
            }
        }

        // clean up work after server is closed
        clientCounter = 0;
        // ? TODO should probably send clients some CLOSED events before ?
        m_clients.clear();

        m_listener.close();
    }
    
    void Server::outThreadUpdate()
    {
        while (isRunning())
        {
            // lock global mutex (also used for adding events to the queue),
            // encode the next event in the queue (if it's not empty) into an outgoing packet,
            // remove the event from the queue, unlock the global mutex
            g_outMutex.lock();

            if (m_outgoingEvents.empty())
            {
                g_outMutex.unlock();
                continue;
            }

            NetworkEvent &event{ *(m_outgoingEvents.front()) };
            int recipient{ event.getRecipient() };

            sf::Packet outPacket;
            outPacket << event;
            m_outgoingEvents.pop_front();

            g_outMutex.unlock();

            // RAII wrapper for mutex
            std::lock_guard<std::mutex> guard(m_clientsMutex);
            // if event has recipient, send event to them, otherwise broadcast the event to everyone
            if (recipient == NetworkEvent::allRecipients)
            {
                for (auto &clientPair : m_clients)
                {
                    auto &client{ *clientPair.second };
                    // send the packet until it's sent completely
                    while (client.send(outPacket) == sf::Socket::Partial) {}
                }
            }
            else
            {
                auto clientIt{ m_clients.find(recipient) };
                if (clientIt != m_clients.end())
                {
                    while (clientIt->second->send(outPacket) == sf::Socket::Partial) {}
                }
            }
        }
    }

    void Server::sendEvent(std::unique_ptr<NetworkEvent> event)
    {
        std::lock_guard<std::mutex> guard(g_outMutex);
        m_outgoingEvents.push_back(std::move(event));
    }

    std::unique_ptr<NetworkEvent> Server::retrieveIncomingEvent()
    {
        std::lock_guard<std::mutex> guard(g_inMutex);
        if (m_incomingEvents.empty())
        {
            // decided not to throw error here, because this method would be triggering every tick,
            // and from what I remember, stack unwinding and catching errors is expensive
            return nullptr;
        }

        std::unique_ptr<NetworkEvent> nextEvent{ std::move(m_incomingEvents.front()) };
        m_incomingEvents.pop_front();
        return nextEvent;
    }
    
}