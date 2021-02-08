#include "Card.h"
#include "Deck.h"
#include "Hand.h"
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

    bool Card::handleEvent(const sf::Event &event, Board &board)
    {
        bool handled{ GameObject::handleEvent(event, board) };

        if (handled)
        {
            return true;
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F && selected)
        {
            flip();
            return true;
        }

        // TODO this thing is questionable
        // 1. Does this card die (in memory) on board.removeLastObject() or what happens?
        // 2. Maybe this is a bit clunky that we delete the card from the board inside this card's method.
        // Could maybe use the collision system in the Board. This would require to move the getBoundingBox() method
        // to GameObject (this probably should be done anyway) and check all the collisions of selected card on mouseButtonReleased
        // 3. Could maybe introduce the version of flip(), which would only show the card for you, but not for the opponent
        // (do this when Players are added)
        if (event.type == sf::Event::MouseButtonReleased && contains(event.mouseButton.x, event.mouseButton.y))
        {
            auto &objectList{ board.getObjectList() };
            for (auto it{ objectList.rbegin() }; it != objectList.rend(); ++it )
            {
                Deck *deck{ dynamic_cast<Deck*>((*it).get()) };

                if (deck && deck->getBoundingBox().intersects(getBoundingBox()))
                {
                    deck->putCardOnTop(*this);
                    board.removeLastObject();
                    break;
                }

                Hand *hand{ dynamic_cast<Hand*>((*it).get()) };

                if (hand && hand->getBoundingBox().intersects(getBoundingBox()))
                {
                    hand->putCardIn(*this);
                    board.removeLastObject();
                    break;
                }
            }

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

}