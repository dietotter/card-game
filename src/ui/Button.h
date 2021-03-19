#pragma once

#include "UIElement.h"
#include "Text.h"

#include <memory>
#include <string>

namespace nik {

    class Button : public UIElement
    {
    private:
        sf::RectangleShape m_rect;
        Text m_buttonText;

        sf::Color m_textColor;
        sf::Color m_pressedTextColor;
        
        bool m_pressed;

    protected:
        virtual void adjustDrawingSize() override;
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        Button();
        Button(int x, int y, float width = 0, float height = 0);
        Button(const Button &button);

        void setButtonText(const Text &text);
        void setTextString(const std::string &string);
        void setCharacterSize(int characterSize);
        void setTextColor(const sf::Color &color);
        void setTextPressedColor(const sf::Color &color);
        void setTextStyle(sf::Uint32 style);
        
        void setOutlineColor(const sf::Color &color);
        void setOutlineThickness(float thickness);

        virtual void setColor(const sf::Color &color) override;
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        virtual bool handleEvent(const sf::Event &event) override;
    };

}