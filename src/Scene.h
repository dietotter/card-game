#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

namespace nik {

    class Scene
    {
    private:
        void commonInputUpdate();
        void commonGraphicsUpdate();

    protected:
        using ChangeSceneFunction = std::function<void(const std::string&)>;
        
        // TODO maybe this should be in SceneManager, which just calls Scene.update(sf::RenderWindow &window)
        sf::RenderWindow &m_window;
        ChangeSceneFunction m_changeScene;

        virtual void inputUpdate(const sf::Event &event) = 0;
        virtual void graphicsUpdate() = 0;

    public:
        Scene(sf::RenderWindow &window, ChangeSceneFunction changeScene): m_window{ window }, m_changeScene{ changeScene } {}

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