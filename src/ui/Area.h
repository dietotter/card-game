#pragma once

#include "UIElement.h"

#include <memory>

namespace nik {

    class Area : public UIElement
    {
    private:
        sf::RectangleShape m_rect;

    protected:
        virtual void adjustDrawingSize() override;
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        Area();
        Area(int x, int y, float width, float height);
        Area(int x, int y, int widthPercent, int heightPercent, float parentWidth, float parentHeight);
        Area(int x, int y, int widthPercent, float height, float parentWidth);
        Area(int x, int y, float width, int heightPercent, float parentHeight);
        Area(const Area &area);

        virtual void setColor(const sf::Color &color) override;
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}