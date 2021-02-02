#pragma once

#include "GameObject.h"
#include "Card.h"

#include <SFML/Graphics.hpp>

#include <vector>

class Hand : public GameObject
{
private:
    std::vector<Card> m_cardList;

    void readjustPosition();

public:
    Hand();

    Card takeCardOut(unsigned int index);
    void putCardIn(const Card& card);

    std::size_t size() const { return m_cardList.size(); }
    bool isEmpty() const { return m_cardList.empty(); }
    sf::FloatRect getBoundingBox() const;

    virtual bool contains(int x, int y) const override;
    virtual bool handleEvent(const sf::Event &event) override;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};