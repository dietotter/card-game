#include "LobbyScene.h"
#include "../../common/SelectionField.h"
#include "../../core/Button.h"
#include "../../core/Text.h"
#include "../../../net/Client.h"
#include "../../../net/Server.h"
#include "../../../net/network-constants.h"
#include "../../../net/events/ConnectionEvent.h"
#include "../../../net/events/LobbyEvent.h"
#include "../../../core-constants.h"
#include "../../../setup.h"

#include <string>
#include <numeric> // for std::accumulate

namespace nik {

    void LobbyScene::clientEventUpdate(NetworkEvent *event)
    {
        if (event->getType() == NetworkEvent::Type::lobbyUpdatePlayersList)
        {
            auto *playersListEvent{ dynamic_cast<UpdatePlayersListEvent*>(event) };
            if (playersListEvent)
            {
                m_players.clear();

                for (auto [id, ready] : playersListEvent->getPlayersList())
                {
                    m_players.push_back({id, ready});
                }

                updatePlayersList();
            }
        }

        if (event->getType() == NetworkEvent::Type::lobbyStartGame)
        {
            auto *startGameEvent{ dynamic_cast<StartGameEvent*>(event) };
            if (startGameEvent)
            {
                this->m_requestScene("OnlineGame", startGameEvent->getParams());
            }
        }
    }

    void LobbyScene::serverEventUpdate(NetworkEvent *event)
    {
        if (event->getType() == NetworkEvent::Type::clientConnected)
        {
            auto *connectedEvent{ dynamic_cast<ClientConnectedEvent*>(event) };
            if (connectedEvent)
            {
                m_players.push_back(LobbyPlayer(connectedEvent->getClientId()));
                Server::sendEvent(std::make_unique<UpdatePlayersListEvent>(m_players));
                addPlayerToList("Player " + std::to_string(connectedEvent->getClientId()));
            }
        }

        if (event->getType() == NetworkEvent::Type::clientDisconnected)
        {
            auto *disconnectedEvent{ dynamic_cast<ClientDisconnectedEvent*>(event) };
            if (disconnectedEvent)
            {
                int id{ disconnectedEvent->getClientId() };
                // erase-remove idiom (see links)
                // https://stackoverflow.com/questions/22729906/stdremove-if-not-working-properly
                // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
                // https://ps-group.github.io/cxx/cxx_remove_if
                m_players.erase(
                    std::remove_if(
                        m_players.begin(),
                        m_players.end(),
                        [id](const LobbyPlayer &player) {
                            return player.getId() == id;
                        }
                    ),
                    m_players.end()
                );
                Server::sendEvent(std::make_unique<UpdatePlayersListEvent>(m_players));

                std::string playerLabel{ "Player " + std::to_string(disconnectedEvent->getClientId()) };

                removePlayerFromList(playerLabel);
            }
        }

        if (event->getType() == NetworkEvent::Type::lobbyPlayerReady)
        {
            auto *playerReadyEvent{ dynamic_cast<PlayerReadyEvent*>(event) };
            if (playerReadyEvent)
            {                
                auto playerId{ playerReadyEvent->getPlayerId() };
                LobbyPlayer &player{ *std::find_if(
                    m_players.begin(),
                    m_players.end(),
                    [playerId](const LobbyPlayer &player) {
                        return player.getId() == playerId;
                    }
                ) };

                player.setReady(playerReadyEvent->isReady());
                player.setDeckString(playerReadyEvent->getDeckString());

                Server::sendEvent(std::make_unique<UpdatePlayersListEvent>(m_players));

                updatePlayersList();
            }
        }
    }

    void LobbyScene::inputUpdate(const sf::Event &event)
    {
        if (event.type == sf::Event::Resized)
        {
            // update the canvas size to the new size of the window (set in Scene class)
            sf::Vector2f windowSize{ m_window.getSize() };
            // TODO if element has been set a percentage-based location, it is not adjusted right now
            m_canvas.setPercentSize(m_canvas.getWidthPercent(), m_canvas.getHeightPercent(), windowSize.x, windowSize.y);
        }

        m_canvas.handleEvent(event);
    }

    void LobbyScene::graphicsUpdate()
    {
        m_window.draw(m_canvas);
    }

    void LobbyScene::initialize(const std::string &params)
    {
        sf::Vector2f windowSize{ m_window.getSize() };
        m_canvas.setPercentSize(100, 100, windowSize.x, windowSize.y);
        m_canvas.setColor(sf::Color(0x281903FF));

        auto quitButton{ std::make_unique<Button>(50, 50, 700, 100) };
        quitButton->setColor(sf::Color(0x550D08FF));
        quitButton->setOutlineThickness(2);
        quitButton->setOutlineColor(sf::Color(0x3E1507FF));
        quitButton->setTextString("Quit lobby");
        quitButton->setCharacterSize(40);
        quitButton->setTextColor(sf::Color(0xD6E0FDFF));
        quitButton->setTextPressedColor(sf::Color(0x929B22FF));
        quitButton->setTextStyle(sf::Text::Bold);

        auto playersList{ std::make_unique<List>(400) };
        playersList->setPosition(50, 200);
        playersList->setName("PlayersList");

        auto deckSelectionField{ std::make_unique<SelectionField>(500, 200, 400, 50) };
        deckSelectionField->setName("DeckSelectionField");
        deckSelectionField->setOptions(setup::loadDeckList());
        deckSelectionField->setPlaceholderString("Choose your deck");
        // TODO BUG: need to do this because without it styles do not apply to the field
        deckSelectionField->setWidth(400);

        auto readyStartButton{ std::make_unique<Button>(800, 50, 700, 100) };
        readyStartButton->setColor(sf::Color(0x550D08FF));
        readyStartButton->setOutlineThickness(2);
        readyStartButton->setOutlineColor(sf::Color(0x3E1507FF));
        readyStartButton->setCharacterSize(40);
        readyStartButton->setTextColor(sf::Color(0xD6E0FDFF));
        readyStartButton->setTextPressedColor(sf::Color(0x929B22FF));
        readyStartButton->setTextStyle(sf::Text::Bold);

        m_role = static_cast<Role>(std::stoi(params));
        if (m_role == Role::server)
        {
            playersList->addItem("Player Server (you)");

            quitButton->onClick = [this](const sf::Event &event) {
                // TODO send some server closed event to connected clients
                Server::stop();
                this->m_requestScene("Menu", "");
                return true;
            };

            readyStartButton->setTextString("Start");

            readyStartButton->onClick = [this](const sf::Event &event) {
                auto deckSelectionIt{ std::find_if(
                    this->m_canvas.getChildren().begin(),
                    this->m_canvas.getChildren().end(),
                    getUIElementNameComparator("DeckSelectionField")
                ) };

                auto deckSelectionField{ dynamic_cast<SelectionField*>(deckSelectionIt->get()) };

                // if player tries to press start when he hasn't selected a deck, nothing happens
                // (or if dynamic cast was unsuccessful)
                // TODO probably start button should be disabled instead
                if (!deckSelectionField || deckSelectionField->getString().isEmpty())
                {
                    // TODO should notify user that he needs to select a deck
                    return true;
                }

                bool allPlayersReady{ std::accumulate(
                    this->m_players.begin(),
                    this->m_players.end(),
                    true,
                    [](bool acc, const LobbyPlayer &player) {
                        return acc && player.isReady();
                    }
                ) };

                // don't start the game if not all players are ready
                if (!allPlayersReady)
                {
                    return true;
                }

                auto deckString{ setup::loadDeckStringFromFile(
                    cnst::deckFolderPath + deckSelectionField->getString() + cnst::deckFileFormat
                ) };

                // create game scene params from role, deck strings and player ids to params
                std::string params{ std::accumulate(
                    this->m_players.begin(),
                    this->m_players.end(),
                    ';' + std::to_string(cnst::serverDefaultId) + ':' + deckString + ';',
                    [](std::string acc, const LobbyPlayer &player) {
                        return acc + std::to_string(player.getId()) + ':' + player.getDeckString() + ';';
                    }
                ) };

                this->m_requestScene("OnlineGame", std::to_string(static_cast<int>(NetworkScene::Role::server)) + params);

                Server::sendEvent(std::make_unique<StartGameEvent>(std::to_string(static_cast<int>(NetworkScene::Role::client)) + params));

                return true;
            };
        }
        else if (m_role == Role::client)
        {
            playersList->addItem("Player Server");

            quitButton->onClick = [this](const sf::Event &event) {
                Client::disconnect();
                this->m_requestScene("Menu", "");
                return true;
            };

            readyStartButton->setTextString("Ready");

            readyStartButton->onClick = [this](const sf::Event &event) {
                auto id{ Client::getIdOnServer() };
                auto &player{ *std::find_if(
                    this->m_players.begin(),
                    this->m_players.end(),
                    [id](const LobbyPlayer &player) {
                        return player.getId() == id;
                    }
                ) };

                auto deckSelectionIt{ std::find_if(
                    this->m_canvas.getChildren().begin(),
                    this->m_canvas.getChildren().end(),
                    getUIElementNameComparator("DeckSelectionField")
                ) };

                auto deckSelectionField{ dynamic_cast<SelectionField*>(deckSelectionIt->get()) };

                // if player tries to press ready when he hasn't selected a deck, nothing happens
                // (or if dynamic cast was unsuccessful)
                // TODO probably ready button should be disabled instead
                if (!deckSelectionField || (!player.isReady() && deckSelectionField->getString().isEmpty()))
                {
                    // TODO should notify user that he needs to select a deck
                    return true;
                }

                auto deckString{ setup::loadDeckStringFromFile(
                    cnst::deckFolderPath + deckSelectionField->getString() + cnst::deckFileFormat
                ) };

                Client::sendEvent(std::make_unique<PlayerReadyEvent>(id, !player.isReady(), deckString));

                return true;
            };
        }

        m_canvas.addChild(std::move(playersList));
        m_canvas.addChild(std::move(deckSelectionField));
        m_canvas.addChild(std::move(quitButton));
        m_canvas.addChild(std::move(readyStartButton));
    }

    std::unique_ptr<Scene> LobbyScene::clone() const
    {
        return std::make_unique<LobbyScene>(*this);
    }

    List* LobbyScene::getPlayersListElement()
    {
        auto playersList{ std::find_if(
            this->m_canvas.getChildren().begin(),
            this->m_canvas.getChildren().end(),
            getUIElementNameComparator("PlayersList")
        ) };

        return dynamic_cast<List*>(playersList->get());
    }

    void LobbyScene::updatePlayersList()
    {
        List *playersList{ getPlayersListElement() };

        if (playersList)
        {
            std::vector<std::string> playerLabels;
            playerLabels.push_back(m_role == Role::server ? "Player Server (you)" : "Player Server");
            
            for (auto &player : m_players)
            {
                std::string playerLabel{ "Player " + std::to_string(player.getId()) + (player.isReady() ? " - READY" : "") };
                if (player.getId() == Client::getIdOnServer())
                {
                    playerLabel += " (you)";
                }

                playerLabels.push_back(playerLabel);
            }

            playersList->updateList(playerLabels);
        }
    }

    void LobbyScene::addPlayerToList(const std::string &label)
    {
        List *playersList{ getPlayersListElement() };

        if (playersList)
        {
            playersList->addItem(label);
        }
    }

    void LobbyScene::removePlayerFromList(const std::string &label)
    {
        List *playersList{ getPlayersListElement() };

        if (playersList)
        {
            playersList->removeItem(label);
            // TODO need to move these string values to be inline constexpr in some header
            playersList->removeItem(label + " - READY");
        }
    }
    
}