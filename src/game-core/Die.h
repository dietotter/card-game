#pragma once

#include "GameObject.h"

namespace nik {

    class Die : public GameObject
    {
    private:
        int m_value{ 1 };
        sf::Sprite m_sprite;

        int roll();
        void adjustTextureRect();

    public:
        Die(): Die(1) {}
        Die(int value);
        
        virtual sf::FloatRect getBoundingBox() const override;
        virtual bool contains(int x, int y) const override;
        virtual bool onSelect(const sf::Event &event, Board &board) override;
        virtual bool onRelease(const sf::Event &event, Board &board) override;
        virtual bool handleEvent(const sf::Event &event, Board &board) override;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}