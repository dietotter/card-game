#include "Client.h"

namespace nik {

    std::thread Client::m_clientThread{};
    sf::TcpSocket Client::m_clientSocket{};
    bool Client::m_connected{};
    
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
        std::thread clientThread{ threadUpdate };
        m_clientThread = std::move(clientThread);
    }

    void Client::disconnect()
    {
        m_connected = false;

        if (m_clientThread.joinable())
        {
            m_clientThread.join();
        }
    }

    void Client::threadUpdate()
    {
        sf::SocketSelector selector;
        selector.add(m_clientSocket);

        while (m_connected)
        {
            if (selector.wait(sf::seconds(1.f)))
            {
                if (selector.isReady(m_clientSocket))
                {
                }
            }
        }

        m_clientSocket.disconnect();
    }

}
