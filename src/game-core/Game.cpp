#include "Game.h"
#include "Card.h"
#include "Deck.h"
#include "Die.h"
#include "Hand.h"
#include "../setup.h"

#include <memory>

namespace nik {

    bool Game::handleUIInput(const sf::Event &event)
    {
        // ? TODO if there will be UI in game ?
        return false;
    }

    bool Game::handleGameInput(const sf::Event &event)
    {
        // spawn a die on board
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && event.key.shift == true)
        {
            m_board.spawnDie(sf::Mouse::getPosition(m_window));
        }

        return m_board.handleEvent(event);
    }

    void Game::inputUpdate(const sf::Event &event)
    {
        bool handled{ handleUIInput(event) };
        if (!handled)
        {
            handleGameInput(event);
        }
    }

    void Game::graphicsUpdate()
    {
        m_window.draw(m_board);
    }

    void Game::initialize()
    {   
        auto &library{ setup::getLibrary() };

        Deck deck{ setup::loadDeckFromFile("data/deck1.dat") };
        auto deckPtr1{ std::make_unique<Deck>(deck) };
        auto deckPtr2{ std::make_unique<Deck>(deck) };
        m_board.addObject(std::move(deckPtr1));
        m_board.addObject(std::move(deckPtr2));

        m_board.addObject(std::make_unique<Card>(library[0]));
        m_board.addObject(std::make_unique<Card>(library[1]));

        auto handPtr{ std::make_unique<Hand>() };
        handPtr->putCardIn(library[0]);
        handPtr->putCardIn(library[0]);
        handPtr->putCardIn(library[1]);
        handPtr->putCardIn(library[2]);

        m_board.addObject(std::move(handPtr));

        m_board.addObject(std::make_unique<Die>());
        m_board.addObject(std::make_unique<Die>(3));
    }
    
}