#include "Server.h"
#include "events/ConnectionEvent.h"
#include "../core-globals.h"

#include <memory>
#include <stdexcept>
#include <iostream>

namespace nik {

    std::thread Server::m_serverInThread{};
    sf::TcpListener Server::m_listener{};
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
                        // RAII wrapper for mutex
                        std::lock_guard<std::mutex> guard(m_clientsMutex);
                        m_clients.insert({++clientCounter, std::move(client)});
                        std::cout << "KEK IS READY\n";
                    }
                }
                else
                {
                    for (auto &clientPair : m_clients)
                    {
                        auto &client{ *clientPair.second };
                        if (selector.isReady(client))
                        {
                            // TODO
                            sf::Packet packet;
                            if (client.receive(packet) == sf::Socket::Done)
                            {
                                std::cout << "PACKET IS READY\n";
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
        // TODO TO FINISH
        while (isRunning())
        {
            // lock global mutex (also used for adding events to the queue),
            // encode the next event in the queue into an outgoing packet,
            // remove the event from the queue, unlock the global mutex
            glob::g_mutex.lock();
            sf::Packet outPacket;
            outPacket << *(m_outgoingEvents.front());
            m_outgoingEvents.pop_front();
            glob::g_mutex.unlock();

            // There probably should be some filtering logic (like the "recipient" field in NetworkEvent, probably),
            // but left it for now
            
            // send the packet with our data to all connected clients
            std::lock_guard<std::mutex> guard(m_clientsMutex);
            for (auto &clientPair : m_clients)
            {
                auto &client{ *clientPair.second };
                client.send(outPacket);
            } 
            
        }
    }
    
}