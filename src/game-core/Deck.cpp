#include "Deck.h"
#include "../core-constants.h"
#include "../core-globals.h"
#include "../setup.h"

#include <algorithm>
#include <exception>
#include <stdexcept>

namespace nik {

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
        std::shuffle(m_cardList.begin(), m_cardList.end(), glob::mt);
    }

    void Deck::putCardOnTop(const Card &card)
    {
        m_cardList.push_back(card);
        Card &addedCard{ m_cardList.back() };

        addedCard.setPosition(0, 0);
        addedCard.faceUp = false;
    }

    void Deck::putCardOnBottom(const Card &card)
    {
        m_cardList.insert(m_cardList.begin(), card);
        Card &addedCard{ m_cardList.front() };

        addedCard.setPosition(0, 0);
        addedCard.faceUp = false;
    }

    Deck& Deck::operator=(const Deck &deck)
    {
        if (this != &deck)
        {
            this->m_cardList = deck.m_cardList;
        }

        return *this;
    }

    sf::FloatRect Deck::getBoundingBox() const
    {
        return { getPosition(), sf::Vector2f(cnst::cardWidth, cnst::cardHeight) };
    }

    bool Deck::contains(int x, int y) const
    {
        // check if mouse is touching the bottom card
        sf::FloatRect bounds{ getPosition(), sf::Vector2f(cnst::cardWidth, cnst::cardHeight) };
        if (bounds.contains(x, y))
        {
            return true;
        }

        int counter{ 1 };
        for (const auto &card : m_cardList)
        {
            // similarly how cards are drawn, check every third and the top card for mouse contact
            if (counter % 3 == 0 || counter == size() - 1)
            {
                bounds.top += 2;
                bounds.left += 2;

                if (bounds.contains(x, y))
                {
                    return true;
                }
            }

            ++counter;
        }

        return false;
    }

    bool Deck::onSelect(const sf::Event &event, Board &board)
    {
        selected = true;
        return true;
    }

    bool Deck::onRelease(const sf::Event &event, Board &board)
    {
        selected = false;
        return true;
    }
    
    bool Deck::handleEvent(const sf::Event &event, Board &board)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type == sf::Event::MouseMoved && selected)
        {
            setPosition(
                event.mouseMove.x - cnst::cardWidth / 2,
                event.mouseMove.y - cnst::cardHeight / 2
            );

            return true;
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T && selected)
        {
            try
            {
                Card card{ takeTopCard() };
                card.setPosition(getPosition());
                board.addObject(std::make_unique<Card>(card));
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }

            return true;
        }

        return false;
    }

    void Deck::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        // draw rect to know the location of the deck when there are no cards in it
        sf::RectangleShape handRect{ sf::Vector2f(cnst::cardWidth, cnst::cardHeight) };
        handRect.setOutlineThickness(2);
        handRect.setOutlineColor(sf::Color::White);
        handRect.setFillColor(sf::Color::Transparent);
        target.draw(handRect, states);

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

        sf::Text cardsInDeck{ std::to_string(counter), setup::getGlobalFont() };
        cardsInDeck.setStyle(sf::Text::Bold);
        states.transform.translate(10, cnst::cardHeight - 40);
        target.draw(cardsInDeck, states);
    }

    std::unique_ptr<GameObject> Deck::clone() const
    {
        return std::make_unique<Deck>(*this);
    }

}