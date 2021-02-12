#include "SceneManager.h"

namespace nik {

    SceneManager::SceneManager(sf::RenderWindow &window):
        m_sceneMap{
            { "Menu", std::make_unique<MenuScene>(window) },
            { "Game", std::make_unique<GameScene>(window) }
        }
    {
        setCurrentScene("Menu");
    }

    const Scene& SceneManager::getCurrentScene()
    {
        return *m_currentScene.get();
    }

    void SceneManager::setCurrentScene(const std::string &sceneName)
    {
        // error: call to implicitly-deleted copy constructor of 'std::__1::unique_ptr<nik::GameObject, std::__1::default_delete<nik::GameObject> >
        m_currentScene = m_sceneMap[sceneName]->clone();
        m_currentScene->initialize();
    }

    void SceneManager::update()
    {
        m_currentScene->update();
    }

}