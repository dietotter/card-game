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
        // TODO std::deque for m_receivedEventsQueue and m_eventsSendingQueue
        // also, maybe separate class for NetworkEvents, with either inner "type" field, depending on which
        // the handling will be performed, or visitor pattern (https://en.wikipedia.org/wiki/Visitor_pattern#Sources)

        static bool m_running;

    public:
        static void listen(int port = cnst::defaultServerPort);
        // TODO if we don't stop server, exception happens on "Quit" button press
        static void stop();

        static void inThreadUpdate();
        static void outThreadUpdate();

        static bool isRunning() { return m_running; }
    };
}