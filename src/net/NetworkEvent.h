#pragma once

#include <SFML/Network.hpp>

#include <memory>

namespace nik {
    
    class NetworkEvent
    {
    public:
        enum class Type
        {
            noType,

            clientConnected,
            clientDisconnected,

            lobbyUpdatePlayersList,
            lobbyPlayerReady,
            lobbyStartGame
        };

        static inline constexpr int allRecipients{ -1 };

    private:
        // TODO package recipient into packets too
        int m_recipient;

    protected:
        Type m_type;
        
        // virtual functions that will be used by operator<< of sf::Packet (to put specific event data in/out)
        virtual void toPacket(sf::Packet &packet) const
        {
        }
        
        virtual void fromPacket(sf::Packet &packet)
        {
        }

    public:
        NetworkEvent(Type type = Type::noType, int recipient = allRecipients): m_type{ type }, m_recipient{ recipient } {}
        virtual ~NetworkEvent() {}

        Type getType() const { return m_type; }
        int getRecipient() const { return m_recipient; }

        friend sf::Packet& operator<<(sf::Packet &packet, const NetworkEvent &event);
        friend sf::Packet& operator>>(sf::Packet &packet, NetworkEvent &event);

        static std::unique_ptr<NetworkEvent> createEventByType(sf::Uint8 type);
    };

    // https://www.sfml-dev.org/tutorials/2.5/network-packet.php#extending-packets-to-handle-user-types
    // inline to avoid multiple definitions
    inline sf::Packet& operator<<(sf::Packet &packet, const NetworkEvent &event)
    {
        packet << static_cast<sf::Uint8>(event.m_type);
        event.toPacket(packet);
        return packet;
    }

    inline sf::Packet& operator>>(sf::Packet &packet, NetworkEvent &event)
    {
        sf::Uint8 type;
        packet >> type;
        event.m_type = static_cast<NetworkEvent::Type>(type);
        event.fromPacket(packet);
        return packet;
    }

}
