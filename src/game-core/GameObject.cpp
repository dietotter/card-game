#include "GameObject.h"
#include "../EventHandler.h"

bool GameObject::handleEvent(const sf::Event &event)
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
        move(event.mouseMove.x - EventHandler::mousePosition.x, event.mouseMove.y - EventHandler::mousePosition.y);

        return true;
    }

    return false;
}