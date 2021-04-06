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
            packet << m_clientId;
        }

        virtual void fromPacket(sf::Packet &packet) override
        {
            packet >> m_clientId;
        }

    public:
        ClientConnectedEvent() = default;
        ClientConnectedEvent(sf::Uint8 clientId)
            : NetworkEvent{ Type::clientConnected }, m_clientId{ clientId }
        {
        }

        int getClientId() const { return m_clientId; }
    };
    
}
