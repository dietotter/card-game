#include "Card.h"
#include "core-constants.h"

Card::Card(int id, const std::string &name, const std::string &description)
    : m_id{ id }, m_name{ name }, m_description{ description }
{
}

bool Card::contains(int x, int y)
{
    sf::FloatRect bounds{ getPosition(), sf::Vector2f(cnst::cardWidth, cnst::cardHeight) };

    return bounds.contains(x, y);
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