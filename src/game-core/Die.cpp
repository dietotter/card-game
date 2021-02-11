#include "Die.h"

#include "../core-constants.h"
#include "../core-globals.h"
#include "../setup.h"

#include <random>

namespace nik {

    Die::Die(int value)
        : m_value{ value }
    {
        m_sprite.setTexture(setup::getDieTexture());
        adjustTextureRect();
    }

    sf::FloatRect Die::getBoundingBox() const
    {
        return { getPosition(), sf::Vector2f(cnst::dieWidth, cnst::dieWidth) };
    }

    bool Die::contains(int x, int y) const
    {
        return getBoundingBox().contains(x, y);
    }

    bool Die::onSelect(const sf::Event &event, Board &board)
    {
        selected = true;
        return true;
    }

    bool Die::onRelease(const sf::Event &event, Board &board)
    {
        selected = false;
        return true;
    }

    bool Die::handleEvent(const sf::Event &event, Board &board)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseMoved && selected)
        {
            setPosition(
                event.mouseMove.x - cnst::dieWidth / 2,
                event.mouseMove.y - cnst::dieWidth / 2
            );

            return true;
        }

        if (event.type == sf::Event::KeyPressed && selected)
        {
            switch(event.key.code)
            {
                case sf::Keyboard::Num1:
                    m_value = 1;
                    break;
                case sf::Keyboard::Num2:
                    m_value = 2;
                    break;
                case sf::Keyboard::Num3:
                    m_value = 3;
                    break;
                case sf::Keyboard::Num4:
                    m_value = 4;
                    break;
                case sf::Keyboard::Num5:
                    m_value = 5;
                    break;
                case sf::Keyboard::Num6:
                    m_value = 6;
                    break;
                case sf::Keyboard::R:
                    roll();
                    break;
                default:
                    break;
            }

            adjustTextureRect();

            return true;
        }

        return false;
    }

    void Die::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_sprite, states);
    }

    int Die::roll()
    {
        // values from 7 to 12 mean that on texture usual values of 1 to 6 will be visible,
        // but with a little "R" inside to indicate that it was randomized
        static std::uniform_int_distribution dieRoll{ 7, 12 };

        m_value = dieRoll(glob::mt);

        return m_value;
    }

    void Die::adjustTextureRect()
    {
        m_sprite.setTextureRect(sf::IntRect(
            cnst::dieWidth * (m_value - 1),
            0,
            cnst::dieWidth,
            cnst::dieWidth
        ));
    }
    
}