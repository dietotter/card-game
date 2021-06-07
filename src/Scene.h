#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

namespace nik {

    class Scene
    {
    public:
        using RequestSceneFunction = std::function<void(const std::string&, const std::string&)>;
        
    private:
        std::string m_name{};

        void commonInputUpdate();
        void commonGraphicsUpdate();

    protected:
        sf::RenderWindow &m_window;
        RequestSceneFunction m_requestScene;

        virtual void networkUpdate() {}
        virtual void inputUpdate(const sf::Event &event) = 0;
        virtual void graphicsUpdate() = 0;

    public:
        Scene(sf::RenderWindow &window, RequestSceneFunction requestScene, const std::string &name = "Default")
            : m_window{ window }, m_requestScene{ requestScene }, m_name{ name } {}

        virtual ~Scene() {}

        // wanted to use params as a class with virtual function with covariant return types,
        // but scratched that idea for now because I don't need a lot of params usage right now
        // (+ I don't know if it would work that way)
        virtual void initialize(const std::string &params) = 0;
        // for polymorphic Scene copying
        virtual std::unique_ptr<Scene> clone() const = 0;
        
        void update()
        {
            networkUpdate();
            commonInputUpdate();
            commonGraphicsUpdate();
        }
    };

}