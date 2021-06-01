#pragma once

#include "../../../net/NetworkScene.h"
#include "../../core/Area.h"
#include "../../common/List.h"
#include "../../../game-core/Player.h"

#include <vector>

namespace nik {

    class LobbyScene : public NetworkScene
    {
    private:
        Area m_canvas;
        std::vector<Player> m_players;
        // TODO gamemode select
        // in lobby screen, there should be:
        // DONE 1. area with players connected (there should be a "(you)" mark near your player's label)
        // 2. deck selection field:
        // a) make additional selection input field for common ui
        // b) move decks to additional "deck" folder in "data", all the decks for selection input should be taken from there
        // c) on "start game", decks selected in deck selection field of all players should be loaded as strings and ?sent to server? to initialize a new game scene with them
        // 3. gamemode selection field (inactive for clients, server is selecting)
        // 4. "start game" available only on all players ready ("ready" button for clients - DONE with bugs)
        List* getPlayersListElement();
        void addPlayerToList(const std::string &label);
        void removePlayerFromList(const std::string &label);
        void updatePlayersList();

    protected:
        virtual void clientEventUpdate(NetworkEvent *event) override;
        virtual void serverEventUpdate(NetworkEvent *event) override;
        virtual void inputUpdate(const sf::Event &event) override;
        virtual void graphicsUpdate() override;

    public:
        LobbyScene(sf::RenderWindow &window, Scene::RequestSceneFunction requestScene): NetworkScene{ window, requestScene, "Lobby" }
        {
        }

        virtual void initialize(const std::string &params) override;
        virtual std::unique_ptr<Scene> clone() const override;

    };

}