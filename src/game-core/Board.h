#pragma once

#include "GameObject.h"
#include "../net/NetworkScene.h"
#include "../core-constants.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace nik {

    class GameObject;

    class Board : public sf::Drawable
    {
        using Role = NetworkScene::Role;
        // unique_ptr because object slicing would occur without using pointers
        using GameObjectList = std::vector<std::unique_ptr<GameObject>>;
        
    private:
        GameObjectList m_objectList;
        
        // using pointer here because references can't be rebound after initialization (so we can't use it in operator=, for example)
        sf::RenderWindow *m_window;
        sf::Color m_backgroundColor;

        void deepCopy(const Board &board);
        bool handleCardDrop(GameObjectList::iterator cardIt);

    public:
        Board(sf::RenderWindow *window): m_window{ window }, m_backgroundColor{ cnst::boardBackgroundColor } {}
        Board(const Board &board);

        const GameObjectList& getObjectList() const { return m_objectList; }
        void addObject(std::unique_ptr<GameObject> gameObject) { m_objectList.push_back(std::move(gameObject)); }

        const sf::RenderWindow* getWindow() const { return m_window; }

        bool handleEvent(const sf::Event &event, Role role = Role::offline);
        void spawnDie(const sf::Vector2i &mousePos);

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        Board& operator=(const Board &board);
    };

}