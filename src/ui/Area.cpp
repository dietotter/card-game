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
            
    Area::Area(int x, int y, int widthPercent, int heightPercent, float parentWidth, float parentHeight)
        : UIElement{ x, y, widthPercent, heightPercent, parentWidth, parentHeight }
    {
        setColor(sf::Color{ 0 });
        adjustDrawingSize();
    }

    Area::Area(int x, int y, int widthPercent, float height, float parentWidth)
            : UIElement{ x, y, widthPercent, height, parentWidth }
    {
        setColor(sf::Color{ 0 });
        adjustDrawingSize();
    }
    
    Area::Area(int x, int y, float width, int heightPercent, float parentHeight)
            : UIElement{ x, y, width, heightPercent, parentHeight }
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
        states.transform *= getTransform();

        target.draw(m_rect);
    }
}