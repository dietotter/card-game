#pragma once

#include "../NetworkEvent.h"

namespace nik {

    class ClientConnectedEvent : public NetworkEvent
    {
    private:
        sf::Uint8 m_clientId{};

    protected:
        virtual void toPacket(sf::Packet &packet) const override
        {
            packet << static_cast<sf::Uint8>(m_type) << m_clientId;
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            sf::Uint8 type;
            packet >> type >> m_clientId;
            m_type = static_cast<Type>(type);
        }

    public:
        ClientConnectedEvent(sf::Uint8 clientId)
            : NetworkEvent{ Type::clientConnected }, m_clientId{ clientId }
        {
        }

        int getClientId() const { return m_clientId; }
    };
    
}
