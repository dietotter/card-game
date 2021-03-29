#pragma once

#include "../../../common/PopUp.h"
#include "../../../../Scene.h"

namespace nik {

    class JoinServerInput : public PopUp
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        JoinServerInput(Scene::RequestSceneFunction requestScene);
    };

}