#pragma once

#include "../Area.h"

namespace nik {

    class Controls : public Area
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        Controls();
    };

}