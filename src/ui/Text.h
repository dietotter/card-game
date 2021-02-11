#pragma once

#include "UIElement.h"

#include <string>
#include <memory>

namespace nik {

    class Text : public UIElement
    {
    private:
        sf::Text m_text;

        void setFont();

    protected:
        virtual void adjustDrawingSize() override;
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        Text();
        Text(const std::string &string);
        Text(int x, int y, const std::string &string = "", int characterSize = 24);
        Text(const Text &text);

        void setString(const std::string &string) { m_text.setString(string); }
        void setCharacterSize(int characterSize) { m_text.setCharacterSize(characterSize); }

        virtual void setColor(const sf::Color &color) override;
        virtual sf::FloatRect getBoundingBox() const override;
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}