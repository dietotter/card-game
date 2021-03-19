#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

namespace nik {

    class Scene
    {
    public:
        using RequestSceneFunction = std::function<void(const std::string&)>;
        
    private:
        std::string m_name{};

        void commonInputUpdate();
        void commonGraphicsUpdate();

    protected:
        // TODO maybe this should be in SceneManager, which just calls Scene.update(sf::RenderWindow &window)
        sf::RenderWindow &m_window;
        RequestSceneFunction m_requestScene;

        virtual void inputUpdate(const sf::Event &event) = 0;
        virtual void graphicsUpdate() = 0;

    public:
        Scene(sf::RenderWindow &window, RequestSceneFunction requestScene, const std::string &name = "Default"): m_window{ window }, m_requestScene{ requestScene }, m_name{ name } {}

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