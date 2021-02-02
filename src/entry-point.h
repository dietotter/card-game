#include "game-core/SetupManager.h"
#include "game-core/core-constants.h"
#include "game-core/Card.h"
#include "game-core/Deck.h"
#include "game-core/GameObject.h"
#include "EventHandler.h"

#include <SFML/Graphics.hpp>

#include <iostream>
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
    std::vector<GameObject*> objectList;

    Deck deck1{ setupManager.loadDeckFromFile("data/deck1.dat") };
    Deck deck2{ deck1 };

    deck2.setPosition(cnst::screenWidth - cnst::cardWidth - 10, cnst::screenHeight - cnst::cardHeight - 10);

    objectList.push_back(&deck1);
    objectList.push_back(&deck2);

    auto &library{ setupManager.getLibrary() };
    Card card1{ library[0] };
    Card card2{ library[1] };

    objectList.push_back(&card1);
    objectList.push_back(&card2);

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