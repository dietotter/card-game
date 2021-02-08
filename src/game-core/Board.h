#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace nik {

    class GameObject;

    class Board : public sf::Drawable
    {
        // unique_ptr because object slicing would occur without using pointers
        using GameObjectList = std::vector<std::unique_ptr<GameObject>>;
        
    private:
        GameObjectList m_objectList;

        sf::Color m_backgroundColor;

    public:
        Board();

        const GameObjectList& getObjectList() const { return m_objectList; }
        void addObject(std::unique_ptr<GameObject> gameObject) { m_objectList.push_back(std::move(gameObject)); }
        void removeLastObject() { m_objectList.pop_back(); }

        bool handleEvent(const sf::Event &event);

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}