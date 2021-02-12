#pragma once

#include "core-constants.h"
#include "SceneManager.h"
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

        SceneManager sceneManager{ window };

        // run the program as long as the window is open
        while (window.isOpen())
        {
            // In future (maybe it'll be done in Game class), it will be also receiving socket updates for multiplayer
            // (probably in a separate thread)
            sceneManager.update();
        }

        return 0;
    }

}