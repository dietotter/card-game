#pragma once

#include "core-constants.h"
#include "game-core/Game.h"
#include "setup.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>
#include <vector>

namespace nik {

    int run()
    {
        if (!setup::initialize())
        {
            return 1;
        }

        sf::RenderWindow window(sf::VideoMode(cnst::screenWidth, cnst::screenHeight), "My window");

        Game gameScene{ window };
        gameScene.initialize();

        // run the program as long as the window is open
        while (window.isOpen())
        {
            // sceneHandler.update (this will draw either menu or game board or deckbuilder depending on the scene,
            // handle events based on the scene, do the logic update based on the scene)
            // In future (maybe it'll be done in Game class), it will be also receiving socket updates for multiplayer
            // (probably in a separate thread)
            gameScene.update();
        }

        return 0;
    }

}