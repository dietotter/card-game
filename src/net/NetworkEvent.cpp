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
        case Type::clientDisconnected:
            return std::make_unique<ClientDisconnectedEvent>();

        case Type::lobbyUpdatePlayersList:
            return std::make_unique<UpdatePlayersListEvent>();
        case Type::lobbyPlayerReady:
            return std::make_unique<PlayerReadyEvent>();
        case Type::lobbyStartGame:
            return std::make_unique<StartGameEvent>();
        
        default:
            return std::make_unique<NetworkEvent>();
        }
    }
    
}