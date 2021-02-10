#pragma once

#include "GameObject.h"
#include "Card.h"
#include "Board.h"

#include <SFML/Graphics.hpp>

#include <vector>

namespace nik {

    class Deck : public GameObject
    {
    private:
        std::vector<Card> m_cardList;

    public:
        Deck() = default;
        Deck(const std::vector<Card> &cardList) : m_cardList{ cardList } {}
        Deck(const Deck &deck) : m_cardList{ deck.m_cardList } {}

        const Card& peek() const;
        Card takeTopCard();
        void shuffle();
        void putCardOnTop(const Card &card);
        void putCardOnBottom(const Card &card);

        std::size_t size() const { return m_cardList.size(); }
        bool isEmpty() const { return m_cardList.empty(); }

        Deck& operator=(const Deck &deck);
        
        virtual sf::FloatRect getBoundingBox() const override;
        virtual bool contains(int x, int y) const override;
        virtual bool onSelect(const sf::Event &event, Board &board) override;
        virtual bool onRelease(const sf::Event &event, Board &board) override;
        virtual bool handleEvent(const sf::Event &event, Board &board) override;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}