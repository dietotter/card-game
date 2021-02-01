#pragma once

#include "game-core/Card.h"

#include <SFML/Graphics.hpp>

class EventHandler
{
private:
    sf::RenderWindow &m_window;
    // we will need to track previous mouse position for card dragging logic
    // TODO (?) shouldn't be a member variable
    sf::Vector2i m_mousePosition;

public:
    EventHandler(sf::RenderWindow &window): m_window{ window }
    {
    }

    // probably will need refactoring when game field is introduced
    void handleWindowEvents() {}
    void handleUIEvents() {}
    void handleGameEvents(std::vector<Card> &library);
};