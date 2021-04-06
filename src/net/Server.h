#pragma once

#include "network-constants.h"
#include "NetworkEvent.h"

#include <SFML/Network.hpp>

#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

namespace nik {

    // making everything static for now, because:
    // 1. To not be stuck on this for too long
    // 2. Haven't come up with the nice idea where and how to store the server instance
    // (probs due to my architecture being wrong for multiplayer from the beginning)
    class Server
    {
    public:
        using EventsQueue = std::deque<std::unique_ptr<NetworkEvent>>;

        // mutexes for in/out event queues
        static std::mutex g_inMutex;
        static std::mutex g_outMutex;

    private:
        // thread for incoming connections and packets
        static std::thread m_serverInThread;
        // thread for sending packets to clients
        static std::thread m_serverOutThread;
        static sf::TcpListener m_listener;
        static std::map<int, std::unique_ptr<sf::TcpSocket>> m_clients;
        static EventsQueue m_incomingEvents;
        static EventsQueue m_outgoingEvents;
        
        // mutex is used for operations on m_clients list by different threads
        static std::mutex m_clientsMutex;

        static bool m_running;

    public:
        static void listen(int port = cnst::defaultServerPort);
        // TODO if we don't stop server, exception happens on "Quit" button press
        static void stop();

        static void inThreadUpdate();
        static void outThreadUpdate();

        static bool isRunning() { return m_running; }

        static EventsQueue& getIncomingEvents() { return m_incomingEvents; }
        static EventsQueue& getOutgoingEvents() { return m_outgoingEvents; }
        
        static void sendEvent(std::unique_ptr<NetworkEvent> event);
        static std::unique_ptr<NetworkEvent> retrieveIncomingEvent();
    };
}