#include "Hand.h"
#include "../core-constants.h"

#include <memory>

namespace nik {

    int Hand::getReadjustedX() const
    {
        // For hand area to be positioned correctly when there are no cards in it (size = 0),
        // we need to recalculate the position of empty area like there is 1 card in Hand
        std::size_t cardsToDraw{ size() };
        if (!cardsToDraw)
        {
            cardsToDraw = 1;
        }

        return (cnst::screenWidth - cardsToDraw * cnst::cardWidth) / 2;
    }

    int Hand::getReadjustedY() const
    {
        return cnst::screenHeight - cnst::cardHeight;
    }

    // readjust position of the hand to be centered, when a card is put into or out of the hand
    void Hand::readjustPosition()
    {
        setPosition(getReadjustedX(), getReadjustedY());
        for (int i{ 0 }; i < size(); ++i)
        {
            m_cardList[i].setPosition(getReadjustedX() + i * cnst::cardWidth, getReadjustedY());
        }
    }
    
    sf::Vector2f Hand::getVisualSize() const
    {
        // For hand area to be visible when there are no cards in it (size = 0),
        // we need to draw the empty area with the size of 1 card
        std::size_t cardsToDraw{ size() };
        if (!cardsToDraw)
        {
            cardsToDraw = 1;
        }

        return { static_cast<float>(cnst::cardWidth * cardsToDraw), cnst::cardHeight };
    }

    Hand::Hand()
    {
        readjustPosition();
    }

    Card Hand::takeCardOut(unsigned int index)
    {
        if (isEmpty())
        {
            throw std::underflow_error("Trying to take a card from an empty hand");
        }

        Card card{ m_cardList[index] };
        m_cardList.erase(m_cardList.begin() + index);

        readjustPosition();

        return card;
    }

    void Hand::putCardIn(const Card &card)
    {
        m_cardList.push_back(card);
        Card &addedCard{ m_cardList.back() };

        readjustPosition();

        addedCard.faceUp = true;
    }

    sf::FloatRect Hand::getBoundingBox() const
    {
        return { getPosition(), getVisualSize() };
    }

    bool Hand::contains(int x, int y) const
    {
        for (const auto &card : m_cardList)
        {
            if (card.contains(x, y))
            {
                return true;
            }
        }

        return false;
    }

    bool Hand::onSelect(const sf::Event &event, Board &board)
    {
        selected = true;
        for (auto &card : m_cardList)
        {
            if (card.contains(event.mouseButton.x, event.mouseButton.y))
            {
                card.selected = true;
                break;
            }
        }

        return true;
    }

    bool Hand::onRelease(const sf::Event &event, Board &board)
    {
        selected = false;
        for (auto &card : m_cardList)
        {
            card.selected = false;
        }

        return true;
    }

    bool Hand::handleEvent(const sf::Event &event, Board &board)
    {
        int index{ 0 };
        for (auto &card : m_cardList)
        {
            bool handled{ card.handleEvent(event, board) };

            // if mouse is released and Card is outside of the Hand, drop it onto the field
            // (otherwise, clip it back to its position in hand)
            if (event.type == sf::Event::MouseButtonReleased && card.contains(event.mouseButton.x, event.mouseButton.y))
            {
                if (card.getBoundingBox().intersects(getBoundingBox()))
                {
                    card.setPosition(getReadjustedX() + index * cnst::cardWidth, getReadjustedY());
                }
                else
                {
                    board.addObject(std::move(std::make_unique<Card>(takeCardOut(index))));
                }
                
                handled = true;
            }

            if (handled)
            {
                return true;
            }

            ++index;
        }

        return false;
    }

    void Hand::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        sf::RectangleShape handRect{ getVisualSize() };
        target.draw(handRect, states);

        for (const auto &card : m_cardList)
        {
            target.draw(card);
        }
    }

}