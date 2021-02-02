#pragma once

#include "game-core/GameObject.h"

#include <SFML/Graphics.hpp>

class EventHandler
{
private:
    sf::RenderWindow &m_window;

public:
    // we will need to track previous mouse position for card dragging logic
    static sf::Vector2i mousePosition;

public:
    EventHandler(sf::RenderWindow &window): m_window{ window }
    {
    }

    // probably will need refactoring when game field is introduced
    void handleWindowEvents() {}
    void handleUIEvents() {}
    void handleGameEvents(std::vector<GameObject*> &objectList);
};