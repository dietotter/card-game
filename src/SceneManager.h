#pragma once

#include "Scene.h"
#include "ui/MenuScene.h"
#include "game-core/GameScene.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <memory>
#include <string>
#include <functional>

namespace nik {

    class SceneManager
    {
    private:
        std::map<std::string, std::unique_ptr<Scene>> m_sceneMap;
        std::unique_ptr<Scene> m_currentScene;
        std::string m_requestedScene;

    public:
        SceneManager(sf::RenderWindow &window);

        SceneManager(const SceneManager &sceneManager) = delete;
        SceneManager& operator=(const SceneManager &sceneManager) = delete;

        const Scene& getCurrentScene();
        void setCurrentScene(const std::string &sceneName);
        static Scene::RequestSceneFunction getRequestSceneChangeFunction(SceneManager &sceneManager);

        void update();
    };

}