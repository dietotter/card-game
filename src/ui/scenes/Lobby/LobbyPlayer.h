#pragma once

#include <string>

namespace nik {

    class LobbyPlayer
    {
    private:
        int m_id{};
        std::string m_deckString;
        bool m_ready{ false };
    
    public:
        LobbyPlayer(int id, bool ready = false): m_id { id }, m_ready{ ready }
        {
        }

        int getId() const { return m_id; }
        const std::string& getDeckString() const { return m_deckString; }
        void setDeckString(const std::string &deckString) { m_deckString = deckString; }
        bool isReady() const { return m_ready; }
        void setReady(bool ready) { m_ready = ready; }
        void flipReady() { m_ready = !m_ready; }
    };
}