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
        return board.handleEvent(event);
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
        m_window.draw(board);
    }

    void Game::initialize()
    {   
        auto &library{ setup::getLibrary() };

        Deck deck{ setup::loadDeckFromFile("data/deck1.dat") };
        auto deckPtr1{ std::make_unique<Deck>(deck) };
        auto deckPtr2{ std::make_unique<Deck>(deck) };
        board.addObject(std::move(deckPtr1));
        board.addObject(std::move(deckPtr2));

        board.addObject(std::make_unique<Card>(library[0]));
        board.addObject(std::make_unique<Card>(library[1]));

        auto handPtr{ std::make_unique<Hand>() };
        handPtr->putCardIn(library[0]);
        handPtr->putCardIn(library[0]);
        handPtr->putCardIn(library[1]);
        handPtr->putCardIn(library[2]);

        board.addObject(std::move(handPtr));

        board.addObject(std::make_unique<Die>());
        board.addObject(std::make_unique<Die>(3));
    }
    
}