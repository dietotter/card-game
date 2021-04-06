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
            clientConnected
        };

        static inline constexpr int allRecipients{ -1 };

    private:
        int m_recipient;

    protected:
        Type m_type;

        NetworkEvent(Type type = Type::noType, int recipient = allRecipients): m_type{ type }, m_recipient{ recipient } {}
        
        // virtual functions that will be used by operator<< of sf::Packet (to put specific event data in/out)
        virtual void toPacket(sf::Packet &packet) const
        {
            packet << static_cast<sf::Uint8>(m_type);
        }
        
        virtual void fromPacket(sf::Packet &packet)
        {
            sf::Uint8 type;
            packet >> type;
            m_type = static_cast<NetworkEvent::Type>(type);
        }

    public:
        virtual ~NetworkEvent();
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
        event.toPacket(packet);
        return packet;
    }

    inline sf::Packet& operator>>(sf::Packet &packet, NetworkEvent &event)
    {
        event.fromPacket(packet);
        return packet;
    }

}
