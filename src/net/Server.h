#pragma once

#include "network-constants.h"

#include <SFML/Network.hpp>

#include <thread>

namespace nik {

    // making everything static for now, because:
    // 1. To not be stuck on this for too long
    // 2. Haven't come up with the nice idea where and how to store the server instance
    // (probs due to my architecture being wrong for multiplayer from the beginning)
    class Server
    {
    private:
        static std::thread m_serverThread;
        static sf::TcpListener m_listener;
        // TODO std::deque for m_receivedEventsQueue and m_eventsSendingQueue
        // also, maybe separate class for NetworkEvents, with either inner "type" field, depending on which
        // the handling will be performed, or visitor pattern (https://en.wikipedia.org/wiki/Visitor_pattern#Sources)

        static bool m_running;

    public:
        static void listen(int port = cnst::defaultServerPort);
        // TODO if we don't stop server, exception happens on "Quit" button press
        static void stop();

        static void threadUpdate();

        static bool isRunning() { return m_running; }
    };
}