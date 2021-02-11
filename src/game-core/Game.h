#pragma once

#include "../Scene.h"
#include "Board.h"

#include <SFML/Graphics.hpp>

namespace nik {

    // TODO
    // this probs will be the class that handles the game progression and setup
    // both in multiplayer and in single player
    // So basically this will be game logic class, and Board is the visual representation + events class
    // maybe Game will have some state of the game, which will be passed to the Board to act accordingly
    // extends Scene

    class Game : public Scene
    {
    private:
        Board m_board;

        bool handleUIInput(const sf::Event &event);
        bool handleGameInput(const sf::Event &event);

    protected:
        virtual void inputUpdate(const sf::Event &event) override;
        virtual void graphicsUpdate() override;

    public:
        Game(sf::RenderWindow &window): Scene{ window }
        {
        }

        virtual void initialize() override;
    };

}