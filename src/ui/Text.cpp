#include "Text.h"

#include "../setup.h"

namespace nik {

    void Text::setFont()
    {
        m_text.setFont(setup::getGlobalFont());
    }

    std::unique_ptr<UIElement> Text::clone() const
    {
        return std::make_unique<Text>(*this);
    }

    void Text::adjustDrawingSize()
    {
    }

    Text::Text()
    {
        setFont();
    }

    Text::Text(const std::string &string)
    {
        m_text.setString(string);
        setFont();
    }

    Text::Text(int x, int y, const std::string &string, int characterSize)
        : UIElement{ x, y }
    {
        m_text.setString(string);
        m_text.setCharacterSize(characterSize);
        setFont();
    }

    Text::Text(const Text &text)
        : UIElement{ text }
    {
    }
    
    void Text::setColor(const sf::Color &color)
    {
        m_text.setFillColor(color);
    }

    sf::FloatRect Text::getBoundingBox() const
    {
        sf::FloatRect bounds{ m_text.getGlobalBounds() };
        // need to do these transormations due to weird bounding box of sf::Text
        bounds.left = getPosition().x;
        bounds.top = getPosition().y;
        bounds.height *= 2;
        return bounds;
    }

    void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_text, states);

        for (const auto &element : m_childrenList)
        {
            target.draw(*element, states);
        }
    }

}