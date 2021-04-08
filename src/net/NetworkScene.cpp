#include "NetworkScene.h"
#include "Client.h"
#include "Server.h"

#include <memory>

namespace nik {

    void NetworkScene::networkUpdate()
    {
        std::unique_ptr<NetworkEvent> event;
        if (m_role == Role::server)
        {
            while ((event = Server::retrieveIncomingEvent()))
            {
                serverEventUpdate(event.get());
            }
        }
        else
        {
            while ((event = Client::retrieveIncomingEvent()))
            {
                clientEventUpdate(event.get());
            }
        }
    }
    
}
