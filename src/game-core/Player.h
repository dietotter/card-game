#pragma once

#include <string>

namespace nik {

    class Player
    {
    private:
        int m_id{};

        // for lobby (? maybe should have different class for lobby player ?)
        std::string m_deckString;
        bool m_ready{ false };
    
    public:
        Player(int id): m_id { id }
        {
        }

        int getId() const { return m_id; }
        const std::string& getDeckString() const { return m_deckString; }
        void setDeckString(const std::string &deckString) { m_deckString = deckString; }
        bool isReady() const { return m_ready; }
        void flipReady() { m_ready = !m_ready; }
    };
}