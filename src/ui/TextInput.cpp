#include "TextInput.h"

namespace nik {

    TextInput::TextInput(): UIElement()
    {
        setDefaultStyle();
    }

    TextInput::TextInput(int x, int y, float width, float height)
        : UIElement{ x, y, width, height }
    {
        setDefaultStyle();
    }
    
    TextInput::TextInput(const TextInput &textInput)
            : UIElement(textInput)
    {
    }

    void TextInput::setColor(const sf::Color &color)
    {
        m_color = color;
        
        if (!m_focused)
        {
            m_rect.setFillColor(color);
        }
    }

    void TextInput::setFocusedColor(const sf::Color &color)
    {
        m_focusedColor = color;
        
        if (m_focused)
        {
            m_rect.setFillColor(color);
        }
    }

    void TextInput::setOutlineColor(const sf::Color &color)
    {
        m_outlineColor = color;
        
        if (!m_focused)
        {
            m_rect.setOutlineColor(color);
        }
    }

    void TextInput::setOutlineFocusedColor(const sf::Color &color)
    {
        m_outlineFocusedColor = color;
        
        if (m_focused)
        {
            m_rect.setOutlineColor(color);
        }
    }

    void TextInput::setOutlineThickness(float thickness)
    {
        m_outlineThickness = thickness;

        if (!m_focused)
        {
            m_rect.setOutlineThickness(thickness);
        }
    }

    void TextInput::setOutlineFocusedThickness(float thickness)
    {
        m_outlineFocusedThickness = thickness;

        if (m_focused)
        {
            m_rect.setOutlineThickness(thickness);
        }
    }

    void TextInput::setCharacterSize(int characterSize)
    {
        m_inputText.setCharacterSize(characterSize);
        m_placeholderText.setCharacterSize(characterSize);
    }

    void TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_hidden)
        {
            states.transform *= getTransform();
            target.draw(m_rect, states);
            
            if (isEmpty())
            {
                target.draw(m_placeholderText, states);
            }
            else
            {
                target.draw(m_inputText, states);
            }

            for (const auto &element : m_childrenList)
            {
                target.draw(*element, states);
            }
        }
    }
    
    bool TextInput::handleEvent(const sf::Event &event)
    {
        if (m_hidden)
        {
            return false;
        }

        for (auto it{ m_childrenList.rbegin() }; it != m_childrenList.rend(); ++it)
        {
            bool handled{ (*it)->handleEvent(event) };
            if (handled)
            {
                return true;
            }
        }
        
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (contains(event.mouseButton.x, event.mouseButton.y))
            {
                if (!m_focused)
                {
                    m_focused = true;
                    m_rect.setFillColor(m_focusedColor);
                    m_rect.setOutlineColor(m_outlineFocusedColor);
                    m_rect.setOutlineThickness(m_outlineFocusedThickness);
                }
            
                return onClick(event);
            }
            else if (!contains(event.mouseButton.x, event.mouseButton.y) && m_focused)
            {
                m_focused = false;
                m_rect.setFillColor(m_color);
                m_rect.setOutlineColor(m_outlineColor);
                m_rect.setOutlineThickness(m_outlineThickness);
            }
        }

        if (event.type == sf::Event::TextEntered && m_focused)
        {
            // Backspace
            if (event.text.unicode == 8)
            {
                sf::String enteredText{ getString() };
                if (!enteredText.isEmpty())
                {
                    m_inputText.setString(enteredText.substring(0, enteredText.toAnsiString().size() - 1));
                }

                return true;
            }

            // Enter
            if (event.text.unicode == '\n')
            {
                onEnterPress(getString().toAnsiString());

                return true;
            }

            m_inputText.setString(getString() + static_cast<char>(event.text.unicode));

            return true;
        }

        return false;
    }

    void TextInput::setDefaultStyle()
    {
        setTextColor(sf::Color::Black);
        setPlaceholderColor(sf::Color{ 200, 200, 200, 255 });

        setColor(sf::Color::White);
        setFocusedColor(sf::Color::White);
        setOutlineThickness(2);
        setOutlineColor(sf::Color{ 200, 200, 200, 255 });
        setOutlineFocusedThickness(2);
        setOutlineFocusedColor(sf::Color::Blue);
    }

    std::unique_ptr<UIElement> TextInput::clone() const
    {
        return std::make_unique<TextInput>(*this);
    }

    void TextInput::adjustDrawingSize()
    {
        m_rect.setSize({ m_width, m_height });
    }
}