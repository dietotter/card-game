#pragma once

#include <string>

namespace nik {

    class Player
    {
    private:
        int m_id{};
        static int m_nextId;

        // for lobby (? maybe should have different class for lobby player ?)
        std::string m_deckString;
        bool m_ready{ false };
    
    public:
        Player(): m_id{ m_nextId++ } {}
        Player(int id): m_id { id } { m_nextId = id + 1; }

        int getId() const { return m_id; }
        const std::string& getDeckString() const { return m_deckString; }
        void setDeckString(const std::string &deckString) { m_deckString = deckString; }
        bool isReady() const { return m_ready; }
        void flipReady() { m_ready = !m_ready; }
    };
}