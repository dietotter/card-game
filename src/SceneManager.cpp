#include "SceneManager.h"

namespace nik {

    SceneManager::SceneManager(sf::RenderWindow &window)
    {
        Scene::RequestSceneFunction requestSceneChange{ getRequestSceneChangeFunction(*this) };
        m_sceneMap.insert({"Menu", std::make_unique<MenuScene>(window, requestSceneChange)});
        m_sceneMap.insert({"Game", std::make_unique<GameScene>(window, requestSceneChange)});
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

    Scene::RequestSceneFunction SceneManager::getRequestSceneChangeFunction(SceneManager &sceneManager)
    {
        return [&sceneManager](const std::string &sceneName) {
            // not making the scene change itself here, because that would be in the middle of previous scene's
            // input handle. So because the previous scene would be destroyed with the scene change,
            // it would crash the program
            sceneManager.m_requestedScene = sceneName;
        };
    }

    void SceneManager::update()
    {
        if (!m_requestedScene.empty())
        {
            setCurrentScene(m_requestedScene);
            m_requestedScene.clear();
        }

        m_currentScene->update();
    }

}