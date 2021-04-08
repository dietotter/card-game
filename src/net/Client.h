#pragma once

#include "network-constants.h"
#include "Server.h"

#include <SFML/Network.hpp>

#include <string>
#include <thread>

namespace nik {

    // pure static class for now - see reasons in Server.h
    class Client
    {
    private:
        static std::thread m_clientInThread;
        static std::thread m_clientOutThread;
        static sf::TcpSocket m_clientSocket;
        static Server::EventsQueue m_incomingEvents;
        static Server::EventsQueue m_outgoingEvents;

        static bool m_connected;
        // id of this client on the connected server
        static int m_idOnServer;

    public:
        static void connectToServer(std::string connectionUrl = ("localhost:" + std::to_string(cnst::defaultServerPort)));
        static void disconnect();

        static void inThreadUpdate();
        static void outThreadUpdate();

        static bool isConnectedToServer() { return m_connected; }
        static int getIdOnServer() { return m_idOnServer; }

        static Server::EventsQueue& getIncomingEvents() { return m_incomingEvents; }
        static Server::EventsQueue& getOutgoingEvents() { return m_outgoingEvents; }
        
        static void sendEvent(std::unique_ptr<NetworkEvent> event);
        static std::unique_ptr<NetworkEvent> retrieveIncomingEvent();
    };

}