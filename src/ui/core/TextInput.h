#pragma once

#include "UIElement.h"
#include "Text.h"

#include <memory>
#include <string>
#include <functional>

namespace nik {

    class TextInput : public UIElement
    {
    private:
        sf::RectangleShape m_rect;
        Text m_inputText;
        Text m_placeholderText;

        sf::Color m_color;
        sf::Color m_focusedColor;
        sf::Color m_outlineColor;
        sf::Color m_outlineFocusedColor;
        float m_outlineThickness{};
        float m_outlineFocusedThickness{};
        
        bool m_focused{};

        void setDefaultStyle();

    protected:
        virtual void adjustDrawingSize() override;
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        TextInput();
        TextInput(int x, int y, float width = 0, float height = 0);
        TextInput(const TextInput &textInput);

        // what will happen if enter is pressed and input field is focused
        std::function<void(const std::string &inputString)> onEnterPress{ [](auto inputString) { } };

        void setPlaceholderString(const std::string &string) { m_placeholderText.setString(string); }
        void setPlaceholderColor(const sf::Color &color) { m_placeholderText.setColor(color); }
        void setPlaceholderStyle(sf::Uint32 style) { m_placeholderText.setStyle(style); }
        
        void setString(const std::string &string) { m_inputText.setString(string); }
        void setTextColor(const sf::Color &color) { m_inputText.setColor(color); }
        void setTextStyle(sf::Uint32 style) { m_inputText.setStyle(style); }
        
        virtual void setColor(const sf::Color &color) override;
        void setFocusedColor(const sf::Color &color);
        void setOutlineColor(const sf::Color &color);
        void setOutlineFocusedColor(const sf::Color &color);
        void setOutlineThickness(float thickness);
        void setOutlineFocusedThickness(float thickness);

        void setCharacterSize(int characterSize);

        const sf::String& getString() const { return m_inputText.getString(); }
        bool isEmpty() const { return getString().isEmpty(); }

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        virtual bool handleEvent(const sf::Event &event) override;
    };

}