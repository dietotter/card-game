#include "EventHandler.h"

#include <algorithm>

sf::Vector2i EventHandler::mousePosition{};

void EventHandler::handleGameEvents(std::vector<std::unique_ptr<GameObject>> &objectList)
{
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            for (auto it{ objectList.begin() }; it != objectList.end(); ++it)
            {
                if ((*it)->contains(event.mouseButton.x, event.mouseButton.y))
                {
                    // move object to the front if it is clicked on
                    std::rotate(objectList.begin(), it, it + 1);
                    break;
                }
            }
        }

        for (auto &gameObject : objectList)
        {
            bool handled{ gameObject->handleEvent(event) };

            if (handled)
            {
                break;
            }
        }

        // update mouse position
        mousePosition = sf::Mouse::getPosition(m_window);
    }
}