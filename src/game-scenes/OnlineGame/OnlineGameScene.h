#pragma once

#include "../../net/NetworkScene.h"
#include "../../game-core/Board.h"
#include "../../game-core/Player.h"

#include <string>
#include <vector>

namespace nik {

    class OnlineGameScene : public NetworkScene
    {
    private:
        std::vector<Player> m_players;
        Board m_board;

        bool handleUIInput(const sf::Event &event);
        bool handleGameInput(const sf::Event &event);

        void initializeFromParams(const std::string &params);

    protected:
        virtual void clientEventUpdate(NetworkEvent *event) override;
        virtual void serverEventUpdate(NetworkEvent *event) override;
        virtual void inputUpdate(const sf::Event &event) override;
        virtual void graphicsUpdate() override;

    public:
        OnlineGameScene(sf::RenderWindow &window, Scene::RequestSceneFunction requestScene):
            NetworkScene{ window, requestScene, "OnlineGame" }, m_board{ &window }
        {
        }

        virtual void initialize(const std::string &params) override;
        virtual std::unique_ptr<Scene> clone() const override;

    };

}