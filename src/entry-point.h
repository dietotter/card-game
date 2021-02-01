#include "game-core/SetupManager.h"
#include "game-core/core-constants.h"
#include "game-core/Card.h"
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

    // run the program as long as the window is open
    while (window.isOpen())
    {
        auto &library{ setupManager.getLibrary() };
        eventHandler.handleGameEvents(library);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (auto it{ library.rbegin() }; it != library.rend(); ++it)
        {
            window.draw(*it);
        }

        // end the current frame
        window.display();
    }

    return 0;
}

// separate the load, draw and event logic (+ some zagotovki for game logic (?))
// 