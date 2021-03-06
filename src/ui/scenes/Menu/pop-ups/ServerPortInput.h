#pragma once

#include "../../../common/PopUp.h"
#include "../../../../Scene.h"

namespace nik {

    class ServerPortInput : public PopUp
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        ServerPortInput(Scene::RequestSceneFunction requestScene);
    };

}