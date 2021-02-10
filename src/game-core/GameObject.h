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

        virtual sf::FloatRect getBoundingBox() const = 0;
        virtual bool contains(int x, int y) const = 0;
        // returns true if event was handled; returns false if not
        // (useful for stopping propagation)
        virtual bool onSelect(const sf::Event &event, Board &board) = 0;
        virtual bool onRelease(const sf::Event &event, Board &board) = 0;
        virtual bool handleEvent(const sf::Event &event, Board &board) = 0;
    };

}