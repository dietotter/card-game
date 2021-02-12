#pragma once

#include <SFML/Graphics.hpp>

namespace nik {

    class Scene
    {
    private:
        void commonInputUpdate();
        void commonGraphicsUpdate();

    protected:
        // TODO maybe this should be in SceneManager, which just calls Scene.update(sf::RenderWindow &window)
        sf::RenderWindow &m_window;

        virtual void inputUpdate(const sf::Event &event) = 0;
        virtual void graphicsUpdate() = 0;

    public:
        Scene(sf::RenderWindow &window): m_window{ window } {}

        virtual ~Scene() {}

        virtual void initialize() = 0;
        // for polymorphic Scene copying
        virtual std::unique_ptr<Scene> clone() const = 0;
        
        void update()
        {
            commonInputUpdate();
            commonGraphicsUpdate();
        }
    };

}