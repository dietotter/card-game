#include "NetworkEvent.h"
#include "events/ConnectionEvent.h"
#include "events/LobbyEvent.h"

namespace nik {

    std::unique_ptr<NetworkEvent> NetworkEvent::createEventByType(sf::Uint8 type)
    {
        Type eventType{ static_cast<Type>(type) };
        switch (eventType)
        {
        case Type::clientConnected:
            return std::make_unique<ClientConnectedEvent>();

        case Type::lobbyUpdatePlayersList:
            return std::make_unique<UpdatePlayersListEvent>();
        
        default:
            return std::make_unique<NetworkEvent>();
        }
    }
    
}