#include "Button.h"
#include "../../setup.h"

namespace nik {

    std::unique_ptr<UIElement> Button::clone() const
    {
        return std::make_unique<Button>(*this);
    }

    void Button::adjustDrawingSize()
    {
        m_rect.setSize({ m_width, m_height });

        // center text inside button
        // TODO this doesn't work correctly, probs bounding box isn't right, leaving it for later
        sf::FloatRect textBounds{ m_buttonText.getBoundingBox() };
        float xOffset{ (m_width - textBounds.width) / 3 };
        float yOffset{ (m_height - textBounds.height) / 4 };
        
        m_buttonText.setPosition(xOffset, yOffset);
    }

    Button::Button(): UIElement()
    {
        setColor(sf::Color{ 255 });
    }

    Button::Button(int x, int y, float width, float height)
        : UIElement{ x, y, width, height }
    {
        setColor(sf::Color{ 255 });
        adjustDrawingSize();
    }
    
    Button::Button(const Button &button)
            : UIElement(button)
    {
    }

    void Button::setButtonText(const Text &text)
    {
        m_buttonText = text;
    }

    void Button::setTextString(const std::string &string)
    {
        m_buttonText.setString(string);
    }

    void Button::setCharacterSize(int characterSize)
    {
        m_buttonText.setCharacterSize(characterSize);
    }

    void Button::setTextColor(const sf::Color &color)
    {
        m_textColor = color;
        m_buttonText.setColor(color);
    }

    void Button::setTextPressedColor(const sf::Color &color)
    {
        m_pressedTextColor = color;
    }

    void Button::setTextStyle(sf::Uint32 style)
    {
        m_buttonText.setStyle(style);
    }
        
    void Button::setOutlineColor(const sf::Color &color)
    {
        m_rect.setOutlineColor(color);
    }

    void Button::setOutlineThickness(float thickness)
    {
        m_rect.setOutlineThickness(thickness);
    }
    
    void Button::setColor(const sf::Color &color)
    {
        m_rect.setFillColor(color);
    }

    void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_hidden)
        {
            states.transform *= getTransform();

            target.draw(m_rect, states);
            target.draw(m_buttonText, states);

            for (const auto &element : m_childrenList)
            {
                target.draw(*element, states);
            }
        }
    }
    
    bool Button::handleEvent(const sf::Event &event)
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
        
        if (event.type == sf::Event::MouseButtonPressed && contains(event.mouseButton.x, event.mouseButton.y))
        {
            m_pressed = true;
            m_buttonText.setColor(m_pressedTextColor);
            return true;
        }

        if (event.type == sf::Event::MouseButtonReleased && m_pressed)
        {
            m_pressed = false;
            m_buttonText.setColor(m_textColor);
            
            if (contains(event.mouseButton.x, event.mouseButton.y))
            {
                return onClick(event);
            }

            return true;
        }

        return false;
    }
}