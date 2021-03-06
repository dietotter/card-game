#include "Client.h"
#include "events/ConnectionEvent.h"
#include "../core-globals.h"

namespace nik {

    std::thread Client::m_clientInThread{};
    std::thread Client::m_clientOutThread{};
    sf::TcpSocket Client::m_clientSocket{};
    Server::EventsQueue Client::m_incomingEvents;
    Server::EventsQueue Client::m_outgoingEvents;
    bool Client::m_connected{};
    int Client::m_idOnServer{};
    
    void Client::connectToServer(std::string connectionUrl)
    {
        // remove any preceding "tcp://" stuff
        std::string delimiter{ "/" };
        size_t pos{ 0 };
        while ((pos = connectionUrl.find(delimiter)) != std::string::npos)
        {
            connectionUrl = connectionUrl.substr(pos + 1, std::string::npos);
        }

        // find url and port on ":" delimiter. If there is none, use default port
        int port{ cnst::defaultServerPort };
        pos = connectionUrl.find(":");
        if (pos != std::string::npos)
        {
            // TODO error check this
            port = std::stoi(connectionUrl.substr(pos + 1, std::string::npos));
            connectionUrl = connectionUrl.substr(0, pos);
        }

        if (isConnectedToServer())
        {
            throw std::runtime_error("You are already connected to server");
        }

        if (m_clientSocket.connect(connectionUrl, port) != sf::Socket::Done)
        {
            throw std::runtime_error("Couldn't connect to " + connectionUrl);
        }

        m_connected = true;
        m_clientInThread = std::thread{ inThreadUpdate };
        m_clientOutThread = std::thread{ outThreadUpdate };
    }

    void Client::disconnect()
    {
        m_connected = false;

        if (m_clientInThread.joinable())
        {
            m_clientInThread.join();
        }

        if (m_clientOutThread.joinable())
        {
            m_clientOutThread.join();
        }
    }

    void Client::inThreadUpdate()
    {
        sf::SocketSelector selector;
        selector.add(m_clientSocket);

        while (isConnectedToServer())
        {
            if (selector.wait(sf::seconds(1.f)))
            {
                if (selector.isReady(m_clientSocket))
                {
                    sf::Packet packet;
                    if (m_clientSocket.receive(packet) == sf::Socket::Done)
                    {
                        sf::Packet packetCopy{ packet };
                        sf::Uint8 type;
                        packetCopy >> type;

                        // client connected event is only going to be sent to client
                        // as a confirmation and id assignment
                        if (type == static_cast<sf::Uint8>(NetworkEvent::Type::clientConnected))
                        {
                            // ? not sure about the global mutex usage here ?
                            std::lock_guard<std::mutex> guard(glob::g_mutex);
                            ClientConnectedEvent event{};
                            packet >> event;
                            m_idOnServer = event.getClientId();
                            continue;
                        }

                        std::lock_guard<std::mutex> guard(Server::g_inMutex);
                        // create event depending on the event type, received from package
                        m_incomingEvents.push_back(NetworkEvent::createEventByType(type));
                        packet >> *m_incomingEvents.back();
                    }
                }
            }
        }

        m_clientSocket.disconnect();
    }

    void Client::outThreadUpdate()
    {
        while (isConnectedToServer())
        {
            Server::g_outMutex.lock();

            if (m_outgoingEvents.empty())
            {
                Server::g_outMutex.unlock();
                continue;
            }

            NetworkEvent &event{ *(m_outgoingEvents.front()) };

            sf::Packet outPacket;
            outPacket << event;
            m_outgoingEvents.pop_front();

            Server::g_outMutex.unlock();

            while (m_clientSocket.send(outPacket) == sf::Socket::Partial) {}
        }
    }

    void Client::sendEvent(std::unique_ptr<NetworkEvent> event)
    {
        std::lock_guard<std::mutex> guard(Server::g_outMutex);
        m_outgoingEvents.push_back(std::move(event));
    }

    std::unique_ptr<NetworkEvent> Client::retrieveIncomingEvent()
    {
        if (m_incomingEvents.empty())
        {
            return nullptr;
        }

        std::lock_guard<std::mutex> guard(Server::g_inMutex);
        std::unique_ptr<NetworkEvent> nextEvent{ std::move(m_incomingEvents.front()) };
        m_incomingEvents.pop_front();
        return nextEvent;
    }

}
