#pragma once

#include <SFML/Graphics.hpp>

#include <ctime>
#include <random>

namespace nik::glob {

    // mersenne twister for random stuff
    static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

    // probably shouldn't be global, but dunno how to store it yet
    class MouseTracker
    {
    public:
        sf::Vector2i previousMousePosition;

        static MouseTracker& getInstance()
        {
            static MouseTracker instance;

            return instance;
        }
    };

}