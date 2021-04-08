#pragma once

#include "../Scene.h"
#include "NetworkEvent.h"

namespace nik {
    
    class NetworkScene : public Scene
    {
    public:
        enum class Role
        {
            server,
            client
        };
    
    protected:
        Role m_role;

    protected:
        NetworkScene(sf::RenderWindow &window, Scene::RequestSceneFunction requestScene, const std::string &name = "Network")
            : Scene{ window, requestScene, name }
        {
        }

        virtual void networkUpdate() override;
        virtual void clientEventUpdate(NetworkEvent *event) = 0;
        virtual void serverEventUpdate(NetworkEvent *event) = 0;

    };

}
