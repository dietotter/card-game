#pragma once

#include <string>

namespace nik {

    class Player
    {
    private:
        int m_id{};
        std::string m_name{};
        int m_positionOnTable{};
    
    public:
        Player(int id, int positionOnTable = 0, const std::string &name = "")
            : m_id{ id }, m_positionOnTable{ positionOnTable }, m_name{ name }
        {
        }

        int getId() const { return m_id; }
        const std::string& getName() const { return m_name; }
        void setName(const std::string &name) { m_name = name; }
        const int getPositionOnTable() const { return m_positionOnTable; }
        void setPositionOnTable(int positionOnTable) { m_positionOnTable = positionOnTable; }
    };
}