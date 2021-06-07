#pragma once

#include "Board.h"
#include "game-constants.h"
#include "../net/NetworkScene.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace nik {

    class Board;

    class GameObject : public sf::Drawable, public sf::Transformable
    {
    protected:
        using Role = NetworkScene::Role;
        int m_ownerId{ cnst::defaultOwnerId };

    public:
        bool selected{ false };

        GameObject(int ownerId = cnst::defaultOwnerId): m_ownerId{ ownerId } {}
        virtual ~GameObject() {}

        virtual sf::FloatRect getBoundingBox() const = 0;
        virtual bool contains(int x, int y) const = 0;
        // returns true if event was handled; returns false if not
        // (useful for stopping propagation)
        virtual bool onSelect(const sf::Event &event, Board &board) = 0;
        virtual bool onRelease(const sf::Event &event, Board &board) = 0;
        virtual bool handleEvent(const sf::Event &event, Board &board, Role role = Role::offline) = 0;

        int getOwnerId() const { return m_ownerId; }
        void setOwnerId(int ownerId) { m_ownerId = ownerId; }

        virtual std::unique_ptr<GameObject> clone() const = 0;
    };

}