#pragma once

#include "../NetworkEvent.h"
#include "../../ui/scenes/Lobby/LobbyPlayer.h"

#include <string>
#include <map>

namespace nik {

    class UpdatePlayersListEvent : public NetworkEvent
    {
    private:
        // key: playerId, value: ready/not ready
        std::map<sf::Uint8, bool> m_playersList;

    protected:
        virtual void toPacket(sf::Packet &packet) const override
        {
            for (auto [id, ready] : m_playersList)
            {
                packet << id << ready;
            }
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            while (!packet.endOfPacket())
            {
                sf::Uint8 id{};
                bool ready{};
                packet >> id >> ready;
                m_playersList.insert({id, ready});
            }
        }

    public:
        UpdatePlayersListEvent(int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyUpdatePlayersList, recipient }
        {
        }

        UpdatePlayersListEvent(const std::vector<LobbyPlayer> &playersList, int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyUpdatePlayersList, recipient }
        {
            for (auto &player : playersList)
            {
                m_playersList.insert({player.getId(), player.isReady()});
            }
        }

        const std::map<sf::Uint8, bool>& getPlayersList() const { return m_playersList; }
    };

    class PlayerReadyEvent : public NetworkEvent
    {
    private:
        sf::Uint8 m_playerId{};
        bool m_ready{};
        std::string m_deckString;

    protected:
        virtual void toPacket(sf::Packet &packet) const override
        {
            packet << m_playerId << m_ready << m_deckString;
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            packet >> m_playerId >> m_ready >> m_deckString;
        }

    public:
        PlayerReadyEvent(sf::Uint8 playerId = 0, bool ready = false, const std::string &deckString = "", int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyPlayerReady, recipient }, m_playerId{ playerId }, m_ready{ ready }, m_deckString{ deckString }
        {
        }

        int getPlayerId() const { return m_playerId; }
        bool isReady() const { return m_ready; }
        const std::string& getDeckString() const { return m_deckString; }
    };

    class StartGameEvent : public NetworkEvent
    {
    private:
        std::string m_params;

    protected:
        virtual void toPacket(sf::Packet &packet) const override
        {
            packet << m_params;
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            packet >> m_params;
        }

    public:
        StartGameEvent(const std::string &params = "", int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::lobbyStartGame, recipient }, m_params{ params }
        {
        }

        const std::string& getParams() const { return m_params; }
    };
    
}
