#pragma once

#include "../NetworkEvent.h"
#include "../../game-core/Player.h"

#include <vector>

namespace nik {

    class UpdatePlayersListEvent : public NetworkEvent
    {
    private:
        std::vector<sf::Uint8> m_playersIdList;

    protected:
        virtual void toPacket(sf::Packet &packet) const override
        {
            for (auto id : m_playersIdList)
            {
                packet << id;
            }
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            while (!packet.endOfPacket())
            {
                sf::Uint8 id{};
                packet >> id;
                m_playersIdList.push_back(id);
            }
        }

    public:
        UpdatePlayersListEvent(int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyUpdatePlayersList, recipient }
        {
        }

        UpdatePlayersListEvent(const std::vector<Player> &playersList, int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyUpdatePlayersList, recipient }
        {
            for (auto &player : playersList)
            {
                m_playersIdList.push_back(player.getId());
            }
        }

        const std::vector<sf::Uint8>& getPlayersIdList() const { return m_playersIdList; }
    };
    
}
