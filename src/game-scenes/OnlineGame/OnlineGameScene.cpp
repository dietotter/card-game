#include "OnlineGameScene.h"
#include "../../game-core/Deck.h"
#include "../../game-core/Hand.h"
#include "../../net/Client.h"
#include "../../net/Server.h"
#include "../../core-constants.h"
#include "../../setup.h"

#include <memory>

namespace nik {

    void OnlineGameScene::clientEventUpdate(NetworkEvent *event)
    {
        if (event->getType() == NetworkEvent::Type::lobbyUpdatePlayersList)
        {
        }
    }

    void OnlineGameScene::serverEventUpdate(NetworkEvent *event)
    {

    }

    bool OnlineGameScene::handleUIInput(const sf::Event &event)
    {
        // ? TODO if there will be UI in game ?
        return false;
    }

    bool OnlineGameScene::handleGameInput(const sf::Event &event)
    {
        // spawn a die on board
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && event.key.shift == true)
        {
            m_board.spawnDie(sf::Mouse::getPosition(m_window));
        }

        return m_board.handleEvent(event, m_role);
    }

    void OnlineGameScene::inputUpdate(const sf::Event &event)
    {
        bool handled{ handleUIInput(event) };
        if (!handled)
        {
            handleGameInput(event);
        }
    }

    void OnlineGameScene::graphicsUpdate()
    {
        m_window.draw(m_board);
    }

    void OnlineGameScene::initialize(const std::string &params)
    {
        auto &library{ setup::getLibrary() };

        initializeFromParams(params);

        // Deck deck{ setup::loadDeckFromFile("data/decks/deck1.dat") };
        // auto deckPtr1{ std::make_unique<Deck>(deck) };
        // auto deckPtr2{ std::make_unique<Deck>(deck) };
        // m_board.addObject(std::move(deckPtr1));
        // m_board.addObject(std::move(deckPtr2));
    }

    std::unique_ptr<Scene> OnlineGameScene::clone() const
    {
        return std::make_unique<OnlineGameScene>(*this);
    }

    void OnlineGameScene::initializeFromParams(const std::string &params)
    {
        std::string str{ params };
        char delimiter{ ';' };

        m_role = static_cast<NetworkScene::Role>(std::stoi(str.substr(0, str.find(delimiter))));
        str.erase(0, 2);
        
        int playerPosition{ 0 };
        size_t pos = 0;
        std::string token;
        int playerId;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            // get the token for one player (id + deck string)
            token = str.substr(0, pos);
            
            // get the player id and add player to the list
            size_t idDelimiterPos{ token.find(':') };
            playerId = std::stoi(token.substr(0, idDelimiterPos));
            token.erase(0, idDelimiterPos + 1);
            m_players.push_back({ playerId, playerPosition });
            
            // create player's deck and hand, apply ownership to hand and deck with cards
            auto deckPtr{ std::make_unique<Deck>(setup::loadDeckFromString(token)) };
            deckPtr->setOwnerId(playerId);
            for (auto &card : deckPtr->getCardList())
            {
                card.setOwnerId(playerId);
            }
            auto handPtr{ std::make_unique<Hand>(m_window, playerPosition) };
            handPtr->setOwnerId(playerId);
            int positionOffset{ playerPosition % 2 == 0 ? -cnst::cardHeight - 10 : cnst::cardHeight + 10 };
            deckPtr->setPosition(handPtr->getPosition().x, handPtr->getPosition().y + positionOffset);

            m_board.addObject(std::move(deckPtr));
            m_board.addObject(std::move(handPtr));
            
            str.erase(0, pos + 1);
            playerPosition++;
        }
    }

}