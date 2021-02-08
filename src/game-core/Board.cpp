#include "Board.h"
#include "../core-constants.h"

#include <algorithm>

namespace nik {

    Board::Board(): m_backgroundColor{ cnst::boardBackgroundColor }
    {
    }

    bool Board::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            // not using reverse iterator because of the std::rotate
            for (auto it{ --m_objectList.end() }; it >= m_objectList.begin(); --it)
            {
                if ((*it)->contains(event.mouseButton.x, event.mouseButton.y))
                {
                    // move object to the front if it is clicked on
                    std::rotate(it, it + 1, m_objectList.end());
                    break;
                }
            }
        }

        for (auto it{ m_objectList.rbegin() }; it != m_objectList.rend(); ++it )
        {
            bool handled{ (*it)->handleEvent(event, *this) };

            if (handled)
            {
                return true;
            }
        }

        return false;
    }

    void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.clear(m_backgroundColor);

        for (auto &gameObject : m_objectList)
        {
            target.draw(*gameObject);
        }
    }

}