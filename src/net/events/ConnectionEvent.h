#pragma once

#include "../NetworkEvent.h"

namespace nik {

    // created on server on client connection, will only be processed by server and sent to
    // the client that has been connected to confirm connection and assign an id
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
        ClientConnectedEvent(sf::Uint8 clientId = 0, int recipient = NetworkEvent::allRecipients)
            : NetworkEvent{ Type::clientConnected, recipient }, m_clientId{ clientId }
        {
        }

        int getClientId() const { return m_clientId; }
    };
    
}
