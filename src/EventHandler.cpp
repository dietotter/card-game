#include "EventHandler.h"

void EventHandler::handleGameEvents(std::vector<Card> &library)
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

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::F)
            {
                for (auto &card : library)
                {
                    if (card.selected)
                    {
                        card.flip();
                        break;
                    }
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            for (auto it{ library.begin() }; it != library.end(); ++it)
            {
                if (it->contains(event.mouseButton.x, event.mouseButton.y))
                {
                    it->selected = true;
                    // move card to the front if it is clicked on
                    std::rotate(library.begin(), it, it + 1);
                    break;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonReleased)
        {
            for (auto &card : library)
            {
                card.selected = false;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {   
            if (event.type == sf::Event::MouseMoved)
            {
                for (auto &card : library)
                {
                    if (card.selected)
                    {
                        card.move(event.mouseMove.x - m_mousePosition.x, event.mouseMove.y - m_mousePosition.y);
                        break;
                    }
                }
            }
        }

        m_mousePosition = sf::Mouse::getPosition(m_window);
    }
}