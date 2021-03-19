#include "Area.h"

namespace nik {

    std::unique_ptr<UIElement> Area::clone() const
    {
        return std::make_unique<Area>(*this);
    }

    Area::Area(): UIElement() { setColor(sf::Color{ 0 }); }

    Area::Area(int x, int y, float width, float height)
        : UIElement{ x, y, width, height }
    {
        setColor(sf::Color{ 0 });
        adjustDrawingSize();
    }
    
    Area::Area(const Area &area)
            : UIElement(area)
    {
    }
    
    void Area::setColor(const sf::Color &color)
    {
        m_rect.setFillColor(color);
    }

    void Area::adjustDrawingSize()
    {
        m_rect.setSize({ m_width, m_height });
    }

    void Area::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_hidden)
        {
            states.transform *= getTransform();

            target.draw(m_rect, states);

            for (const auto &element : m_childrenList)
            {
                target.draw(*element, states);
            }
        }
    }
}