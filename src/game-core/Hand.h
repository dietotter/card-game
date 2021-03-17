#pragma once

#include "GameObject.h"
#include "Card.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

namespace nik {

    class Hand : public GameObject
    {
    private:
        std::vector<Card> m_cardList;

        int getReadjustedX() const;
        int getReadjustedY() const;
        void readjustPosition();
        sf::Vector2f getVisualSize() const;

    public:
        Hand();

        Card takeCardOut(unsigned int index);
        void putCardIn(const Card& card);

        std::size_t size() const { return m_cardList.size(); }
        bool isEmpty() const { return m_cardList.empty(); }

        virtual sf::FloatRect getBoundingBox() const override;
        virtual bool contains(int x, int y) const override;
        virtual bool onSelect(const sf::Event &event, Board &board) override;
        virtual bool onRelease(const sf::Event &event, Board &board) override;
        virtual bool handleEvent(const sf::Event &event, Board &board) override;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        virtual std::unique_ptr<GameObject> clone() const override;
    };

}