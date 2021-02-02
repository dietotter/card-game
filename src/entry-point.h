#include "game-core/SetupManager.h"
#include "game-core/core-constants.h"
#include "game-core/Card.h"
#include "game-core/Deck.h"
#include "game-core/Hand.h"
#include "game-core/GameObject.h"
#include "EventHandler.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>
#include <vector>

int run()
{
    SetupManager setupManager;
    if (!setupManager.initialize())
    {
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(cnst::screenWidth, cnst::screenHeight), "My window");
    EventHandler eventHandler{ window };

    // TODO when Game (or Field or whatever) class is introduced, this should be moved there
    // making this unique_ptr so that object slicing can be avoided
    std::vector<std::unique_ptr<GameObject>> objectList;

    objectList.push_back(std::make_unique<Deck>(setupManager.loadDeckFromFile("data/deck1.dat")));
    // temporary dynamic casting, decks probably won't be copied like this
    objectList.push_back(std::make_unique<Deck>(*dynamic_cast<Deck*>(objectList[0].get())));
    
    objectList[1]->setPosition(cnst::screenWidth - cnst::cardWidth - 10, cnst::screenHeight - cnst::cardHeight - 10);

    auto &library{ setupManager.getLibrary() };

    objectList.push_back(std::make_unique<Card>(library[0]));
    objectList.push_back(std::make_unique<Card>(library[1]));

    objectList.push_back(std::make_unique<Hand>());

    Hand &hand{ *dynamic_cast<Hand*>(objectList[4].get()) };
    hand.putCardIn(library[0]);
    hand.putCardIn(library[0]);
    hand.putCardIn(library[1]);
    hand.putCardIn(library[2]);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        eventHandler.handleGameEvents(objectList);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (auto it{ objectList.rbegin() }; it != objectList.rend(); ++it)
        {
            window.draw(**it);
        }

        // end the current frame
        window.display();
    }

    return 0;
}

// separate the load, draw and event logic (+ some zagotovki for game logic (?))
// 