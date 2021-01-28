#include "src/game-core/SetupManager.h"
#include "src/game-core/core-constants.h"

#include <SFML/Graphics.hpp>

#include <iostream>

// TODO move it from main to some entry point class, and call that class from main (?)
int main()
{
    SetupManager setupManager;
    if (!setupManager.initialize())
    {
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(cnst::screenWidth, cnst::screenHeight), "My window");

    // TODO move event handling to another place
    sf::Vector2i lastMousePosition;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F)
                {
                    for (auto &card : setupManager.getLibrary())
                    {
                        if (card.selected)
                        {
                            card.flip();
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                for (auto &card : setupManager.getLibrary())
                {
                    if (card.contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        card.selected = true;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                for (auto &card : setupManager.getLibrary())
                {
                    card.selected = false;
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {   
                if (event.type == sf::Event::MouseMoved)
                {
                    for (auto &card : setupManager.getLibrary())
                    {
                        if (card.selected)
                        {
                            card.move(event.mouseMove.x - lastMousePosition.x, event.mouseMove.y - lastMousePosition.y);
                            break;
                        }
                    }
                }
            }

            lastMousePosition = sf::Mouse::getPosition(window);
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        for (auto &card : setupManager.getLibrary())
        {
            // if (card.getId() == 1)
            // {
            //     card.faceUp = false;
            //     card.faceDownSprite.setPosition(400, 200);
            // }
            // card.faceUpSprite.setPosition(card.getId() * 400, card.getId() * 200);
            window.draw(card);
        }

        // end the current frame
        window.display();
    }

    return 0;
}

// TODO read and complete the TODO's above
// (?) separate taki Card from CardInfo, moving description, id and name into card info (and constructing using it), and the rest (+ id) to Card, and that will go to some library vector, which will be used to create in-game copies of cards. Also, library vector should probably be moved in some other class
// separate the load, draw and event logic (+ some zagotovki for game logic (?))
// 