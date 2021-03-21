#pragma once

#include "../components/PopUp.h"

namespace nik {

    class Controls : public PopUp
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        Controls();
    };

}