#include "Hand.h"
#include "core-constants.h"

int Hand::getReadjustedX()
{
    return (cnst::screenWidth - size() * cnst::cardWidth) / 2;
}

int Hand::getReadjustedY()
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

Hand::Hand()
{
    readjustPosition();
}

Card Hand::takeCardOut(unsigned int index)
{
    if (isEmpty())
    {
        throw std::underflow_error("Trying to take a card from an empty deck");
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

bool Hand::handleEvent(const sf::Event &event)
{
    int index{ 0 };
    for (auto &card : m_cardList)
    {
        bool handled{ card.handleEvent(event) };

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
                // TODO need to take card out onto the field
                // two ideas now:
                // 1. The card is just left outside of the zone in this frame,
                // and there is external collision (or something) checker in the Game (or Board or whatever) class,
                // which will detect whether it toucher the board, deck, hand or smth else (and at some point also perform the owner check)
                // and act accordingly (take card out of hand, add it to the board)
                // 2. Game (Board etc) is a member reference variable on every gameobject
                // OR it is passed in handleEvent. This way, I could handle the taking out onto the field thing right here
                takeCardOut(index);
                handled = true;
            }
        }

        if (handled)
        {
            return true;
        }

        ++index;
    }

    return false;
}

sf::FloatRect Hand::getBoundingBox() const
{
    return { getPosition(), sf::Vector2f(cnst::cardWidth * size(), cnst::cardHeight) };
}

void Hand::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    sf::RectangleShape handRect{ sf::Vector2f(cnst::cardWidth * size(), cnst::cardHeight) };
    target.draw(handRect, states);

    for (const auto &card : m_cardList)
    {
        target.draw(card);
    }
}