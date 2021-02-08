#pragma once

#include "Board.h"

#include <SFML/Graphics.hpp>

namespace nik {

    class Board;

    class GameObject : public sf::Drawable, public sf::Transformable
    {
    public:
        bool selected{ false };

        virtual ~GameObject() {}

        virtual bool contains(int x, int y) const = 0;
        // returns true if event was handled; returns false if not
        // (useful for stopping propagation)
        virtual bool handleEvent(const sf::Event &event, Board &board);
    };

}