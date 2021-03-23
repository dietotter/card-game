#include "Server.h"

#include <memory>
#include <stdexcept>
#include <vector>

namespace nik {

    std::thread Server::m_serverThread{};
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
        std::thread serverThread{ threadUpdate };
        m_serverThread = std::move(serverThread);
    }

    void Server::stop()
    {
        m_running = false;

        if (m_serverThread.joinable())
        {
            m_serverThread.join();
        }
    }

    void Server::threadUpdate()
    {
        sf::SocketSelector selector;
        selector.add(m_listener);

        std::vector<std::unique_ptr<sf::TcpSocket>> clients;

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
                        clients.push_back(std::move(client));
                    }
                }
            }
            else
            {
                for (auto &client : clients)
                {
                    if (selector.isReady(*client))
                    {
                        // TODO
                        sf::Packet packet;
                        if (client->receive(packet) == sf::Socket::Done)
                        {
                            
                        }
                    }
                }
            }
        }
    }
    
}