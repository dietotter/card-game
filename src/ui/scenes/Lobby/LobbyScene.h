#pragma once

#include "LobbyPlayer.h"
#include "../../../net/NetworkScene.h"
#include "../../core/Area.h"
#include "../../common/List.h"

#include <vector>

namespace nik {

    class LobbyScene : public NetworkScene
    {
    private:
        // TODO UIScene class, which has a m_canvas (and all the scenes that use UI inherit that class)
        Area m_canvas;
        std::vector<LobbyPlayer> m_players;
        // TODO gamemode select
        // in lobby screen, there should be:
        // DONE 1. area with players connected (there should be a "(you)" mark near your player's label)
        // DONE 2. deck selection field:
        // DONE a) make additional selection input field for common ui
        // DONE b) move decks to additional "deck" folder in "data", all the decks for selection input should be taken from there
        // DONE c) on "start game", decks selected in deck selection field of all players should be loaded as strings and ?sent to server? to initialize a new game scene with them
        // 3. gamemode selection field (inactive for clients, server is selecting)
        // DONE 4. "start game" available only on all players ready ("ready" button for clients)
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