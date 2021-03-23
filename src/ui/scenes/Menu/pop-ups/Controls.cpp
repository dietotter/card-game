#include "Controls.h"

#include "../../../core/Text.h"

#include <SFML/Graphics.hpp>

namespace nik {

    std::unique_ptr<UIElement> Controls::clone() const
    {
        return std::make_unique<Controls>(*this);
    }

    Controls::Controls(): PopUp()
    {
        auto controlsText{ std::make_unique<Text>(100, 100) };
        controlsText->setCharacterSize(40);
        controlsText->setLineSpacing(2.f);
        controlsText->setString("Drag + drop - put a card in/out of a deck/hand\nClick + T - take the top card out of a deck\nClick + F - flip a card\nShift + D - spawn a die\nClick + D - delete a die\nClick + R - roll a die\nClick + 1/2/3/4/5/6 - assign value to a die");
        
        addChild(std::move(controlsText));
    }

}