#pragma once

#include <SFML/Network.hpp>

namespace nik {
    
    class NetworkEvent
    {
    public:
        enum class Type
        {
            clientConnected
        };

    protected:
        Type m_type;
        
        NetworkEvent(Type type): m_type{ type } {}
        
        // virtual functions that will be used by operator<< of sf::Packet (to put specific event data in/out)
        virtual void toPacket(sf::Packet &packet) const = 0;
        virtual void fromPacket(sf::Packet &packet) = 0;

    public:
        virtual ~NetworkEvent();
        Type getType() const { return m_type; }

        friend sf::Packet& operator<<(sf::Packet &packet, const NetworkEvent &event);
        friend sf::Packet& operator>>(sf::Packet &packet, NetworkEvent &event);
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
