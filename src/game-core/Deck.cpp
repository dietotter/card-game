#include "Deck.h"
#include "RandomCore.h"

#include <algorithm>
#include <stdexcept>

const Card& Deck::peek() const
{
    if (isEmpty())
    {
        throw std::underflow_error("Trying to peek an empty deck");
    }

    return m_cardList.back();
}

Card Deck::takeTopCard()
{
    if (isEmpty())
    {
        throw std::underflow_error("Trying to take a card from an empty deck");
    }

    Card topCard{ m_cardList.back() };
    m_cardList.pop_back();

    return topCard;
}

void Deck::shuffle()
{
    std::shuffle(m_cardList.begin(), m_cardList.end(), rnd::RandomCore::mt);
}

void Deck::putCardOnTop(Card card)
{
    card.setOrigin(getPosition());
    card.setPosition(0, 0);
    card.faceUp = false;
    m_cardList.push_back(card);
}

void Deck::putCardOnBottom(Card card)
{
    card.setOrigin(getPosition());
    card.setPosition(0, 0);
    card.faceUp = false;
    m_cardList.insert(m_cardList.begin(), card);
}

Deck& Deck::operator=(const Deck &deck)
{
    if (this != &deck)
    {
        this->m_cardList = deck.m_cardList;
    }

    return *this;
}

void Deck::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    int counter{ 0 };
    for (const auto &card : m_cardList)
    {
        // draw bottom card
        if (counter == 0)
        {
            target.draw(card, states);
        }
        // draw top card and every third card
        // draw them with slight offset to imitate deck depth
        else if (counter % 3 == 0 || counter == size() - 1)
        {
            states.transform.translate(2, 2);

            target.draw(card, states);
        }

        ++counter;
    }
}