#pragma once

#include "UIElement.h"

#include <memory>
#include <string>

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
        Area(int x, int y, float width = 0, float height = 0);
        Area(const Area &area);

        virtual void setColor(const sf::Color &color) override;
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}