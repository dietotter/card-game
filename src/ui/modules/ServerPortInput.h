#pragma once

#include "../components/PopUp.h"

namespace nik {

    class ServerPortInput : public PopUp
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        ServerPortInput();
    };

}