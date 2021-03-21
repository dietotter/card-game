#pragma once

#include "../Area.h"

namespace nik {

    class PopUp : public Area
    {
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        PopUp();
        
        virtual bool handleEvent(const sf::Event &event) override;
    };

}