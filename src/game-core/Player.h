#pragma once

#include <string>

namespace nik {

    class Player
    {
    private:
        int m_id{};
        static int m_nextId;

        std::string m_deckString;
    
    public:
        Player(): m_id{ m_nextId++ } {}
        Player(int id): m_id { id } { m_nextId = id + 1; }

        int getId() { return m_id; }
        const std::string& getDeckString() { return m_deckString; }
        void setDeckString(const std::string &deckString) { m_deckString = deckString; }
    };
}