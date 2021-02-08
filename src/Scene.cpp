#include "Scene.h"

#include "core-globals.h"

namespace nik {

    void Scene::commonInputUpdate()
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
            }

            // handle scene-specific input
            inputUpdate(event);

            // update mouse position
            glob::MouseTracker::getInstance().previousMousePosition = sf::Mouse::getPosition(m_window);
        }
    }

    void Scene::commonGraphicsUpdate()
    {
        m_window.clear(sf::Color::Black);

        // draw scene-specific stuff
        graphicsUpdate();

        // end the current frame (maybe this should be outside in the main window loop)
        m_window.display();
    }

}