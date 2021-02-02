#include "Card.h"
#include "core-constants.h"

Card::Card(int id, const std::string &name, const std::string &description)
    : m_id{ id }, m_name{ name }, m_description{ description }
{
}

sf::FloatRect Card::getBoundingBox() const
{
    return { getPosition(), sf::Vector2f(cnst::cardWidth, cnst::cardHeight) };
}

bool Card::contains(int x, int y) const
{
    return getBoundingBox().contains(x, y);
}

bool Card::handleEvent(const sf::Event &event)
{
    bool handled{ GameObject::handleEvent(event) };

    if (handled)
    {
        return true;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F && selected)
    {
        flip();
        return true;
    }

    return false;
}

void Card::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if (faceUp)
    {
        target.draw(m_faceUpSprite, states);
    }
    else
    {
        target.draw(m_faceDownSprite, states);
    }
}