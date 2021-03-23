#include "Text.h"

#include "../../setup.h"

namespace nik {

    void Text::initializeFont()
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
        initializeFont();
    }

    Text::Text(const std::string &string)
    {
        m_text.setString(string);
        initializeFont();
    }

    Text::Text(int x, int y, const std::string &string, int characterSize)
        : UIElement{ x, y }
    {
        m_text.setString(string);
        m_text.setCharacterSize(characterSize);
        initializeFont();
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
        bounds.left = getActualPosition().x;
        bounds.top = getActualPosition().y;
        bounds.height *= 2;
        return bounds;
    }

    void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        if (!m_hidden)
        {
            states.transform *= getTransform();

            target.draw(m_text, states);

            for (const auto &element : m_childrenList)
            {
                target.draw(*element, states);
            }
        }
    }

}