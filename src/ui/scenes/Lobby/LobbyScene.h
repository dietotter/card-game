#pragma once

#include "../../../Scene.h"
#include "../../core/Area.h"

namespace nik {

    class LobbyScene : public Scene
    {
    private:
        Area m_canvas;

    protected:
        virtual void inputUpdate(const sf::Event &event) override;
        virtual void graphicsUpdate() override;

    public:
        LobbyScene(sf::RenderWindow &window, Scene::RequestSceneFunction requestScene): Scene{ window, requestScene, "Lobby" }
        {
        }

        virtual void initialize() override;
        virtual std::unique_ptr<Scene> clone() const override;

    };

}