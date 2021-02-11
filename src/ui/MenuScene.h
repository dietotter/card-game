#pragma once

#include "../Scene.h"
#include "Area.h"

#include <SFML/Graphics.hpp>

namespace nik {

    class MenuScene : public Scene
    {
    private:
        Area m_canvas;

    protected:
        virtual void inputUpdate(const sf::Event &event) override;
        virtual void graphicsUpdate() override;

    public:
        MenuScene(sf::RenderWindow &window): Scene{ window }
        {
        }

        virtual void initialize() override;
    };

}