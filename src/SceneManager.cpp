#include "SceneManager.h"

namespace nik {

    SceneManager::SceneManager(sf::RenderWindow &window)
    {
        m_sceneMap.insert({"Menu", std::make_unique<MenuScene>(window)});
        m_sceneMap.insert({"Game", std::make_unique<GameScene>(window)});
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

    void SceneManager::update()
    {
        m_currentScene->update();
    }

}