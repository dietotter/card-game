#include "Card.h"
#include "../core-constants.h"

namespace nik {

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

    bool Card::onSelect(const sf::Event &event, Board &board)
    {
        selected = true;
        return true;
    }

    bool Card::onRelease(const sf::Event &event, Board &board)
    {
        selected = false;
        return true;
    }

    bool Card::handleEvent(const sf::Event &event, Board &board)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseMoved && selected)
        {
            setPosition(
                event.mouseMove.x - cnst::cardWidth / 2,
                event.mouseMove.y - cnst::cardHeight / 2
            );

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

    std::unique_ptr<GameObject> Card::clone() const
    {
        return std::make_unique<Card>(*this);
    }

}