#include "SceneManager.h"
#include "game-core/GameScene.h"
#include "ui/scenes/Lobby/LobbyScene.h"
#include "ui/scenes/Menu/MenuScene.h"

namespace nik {

    SceneManager::SceneManager(sf::RenderWindow &window)
    {
        Scene::RequestSceneFunction requestSceneChange{ getRequestSceneChangeFunction(*this) };
        m_sceneMap.insert({"Game", std::make_unique<GameScene>(window, requestSceneChange)});
        m_sceneMap.insert({"Lobby", std::make_unique<LobbyScene>(window, requestSceneChange)});
        m_sceneMap.insert({"Menu", std::make_unique<MenuScene>(window, requestSceneChange)});
        setCurrentScene("Menu");
    }

    const Scene& SceneManager::getCurrentScene()
    {
        return *m_currentScene.get();
    }

    void SceneManager::setCurrentScene(const std::string &sceneName)
    {
        m_currentScene = m_sceneMap[sceneName]->clone();
        m_currentScene->initialize(m_requestedSceneParams);
    }

    Scene::RequestSceneFunction SceneManager::getRequestSceneChangeFunction(SceneManager &sceneManager)
    {
        // lambdas that capture something can't have default arguments (https://en.cppreference.com/w/cpp/language/default_arguments)
        // that's why couldn't give "params" a default value
        return [&sceneManager](const std::string &sceneName, const std::string &params) {
            // not making the scene change itself here, because that would be in the middle of previous scene's
            // input handle. So because the previous scene would be destroyed with the scene change,
            // it would crash the program
            sceneManager.m_requestedScene = sceneName;
            sceneManager.m_requestedSceneParams = params;
        };
    }

    void SceneManager::update()
    {
        if (!m_requestedScene.empty())
        {
            setCurrentScene(m_requestedScene);
            m_requestedScene.clear();
            m_requestedSceneParams.clear();
        }

        m_currentScene->update();
    }

}