#include "SceneManager.h"

namespace nik {

    SceneManager::SceneManager(sf::RenderWindow &window)
    {
        Scene::ChangeSceneFunction changeScene{ getChangeSceneFunction(*this) };
        // these 2 lines currently invoke the "reference to non-static member function must be called" error
        // (https://stackoverflow.com/questions/26331628/reference-to-non-static-member-function-must-be-called)
        // also, some interesting read found on that page:
        // https://isocpp.org/wiki/faq/pointers-to-members#fnptr-vs-memfnptr-types
        // 2 ways to fix i thought of right now:
        // 1. make SceneManager static
        // 2. make setCurrentScene (rename this if going this way) take in SceneManager& argument and return
        // a function, which takes in sceneName argument and changes the scene. Then, call the setCurrentScene
        // in these two lines with *this in parameters
        m_sceneMap.insert({"Menu", std::make_unique<MenuScene>(window, changeScene)});
        m_sceneMap.insert({"Game", std::make_unique<GameScene>(window, changeScene)});
        setCurrentScene("Menu");
    }

    const Scene& SceneManager::getCurrentScene()
    {
        return *m_currentScene.get();
    }

    void SceneManager::setCurrentScene(const std::string &sceneName)
    {
        m_currentScene = m_sceneMap[sceneName]->clone();
        m_currentScene->initialize();
    }

    Scene::ChangeSceneFunction SceneManager::getChangeSceneFunction(SceneManager &sceneManager)
    {
        return [&sceneManager](const std::string &sceneName) {
            sceneManager.setCurrentScene(sceneName);
        };
    }

    void SceneManager::update()
    {
        m_currentScene->update();
    }

}