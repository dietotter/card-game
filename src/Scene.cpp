#include "Scene.h"

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

            // maybe this shouldn't be in menu scenes
            if (event.type == sf::Event::Resized)
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                m_window.setView(sf::View(visibleArea));
            }

            // handle scene-specific put
            inputUpdate(event);
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