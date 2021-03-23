#pragma once

#include "network-constants.h"

#include <SFML/Network.hpp>

#include <string>
#include <thread>

namespace nik {

    // pure static class for now - see reasons in Server.h
    class Client
    {
    private:
        static std::thread m_clientThread;
        static sf::TcpSocket m_clientSocket;

        static bool m_connected;

    public:
        static void connectToServer(std::string connectionUrl = ("localhost" + std::to_string(cnst::defaultServerPort)));
        static void disconnect();

        static void threadUpdate();

        static bool isConnectedToServer() { return m_connected; }
    };

}