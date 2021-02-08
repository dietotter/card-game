#include "GameObject.h"
#include "../core-globals.h"

namespace nik {

    bool GameObject::handleEvent(const sf::Event &event, Board &board)
    {
        if (event.type == sf::Event::MouseButtonPressed && contains(event.mouseButton.x, event.mouseButton.y))
        {
            selected = true;

            return true;
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            selected = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseMoved && selected)
        {
            move(
                event.mouseMove.x - glob::MouseTracker::getInstance().previousMousePosition.x,
                event.mouseMove.y - glob::MouseTracker::getInstance().previousMousePosition.y
            );

            return true;
        }

        return false;
    }

}