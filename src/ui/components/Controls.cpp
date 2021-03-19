#include "Controls.h"

#include "../Button.h"
#include "../Text.h"

#include <SFML/Graphics.hpp>

namespace nik {

    std::unique_ptr<UIElement> Controls::clone() const
    {
        return std::make_unique<Controls>(*this);
    }

    Controls::Controls(): Area()
    {
        setColor(sf::Color(0, 0, 0, 200));

        // turn off propagation
        onClick = [](auto event) { return true; };
        
        auto closeButton { std::make_unique<Button>(0, 0, 50, 50) };
        closeButton->setColor(sf::Color::Transparent);
        closeButton->onClick = [this](const sf::Event &event) {
            this->setHidden(true);
            return true;
        };
        closeButton->setTextString("x");
        closeButton->setCharacterSize(40);
        closeButton->setTextColor(sf::Color::White);
        closeButton->setTextPressedColor(sf::Color(200, 200, 200, 255));

        auto controlsText{ std::make_unique<Text>(100, 100) };
        controlsText->setCharacterSize(40);
        controlsText->setLineSpacing(2.f);
        controlsText->setString("Drag + drop - put a card in/out of a deck/hand\nClick + T - take the top card out of a deck\nClick + F - flip a card\nShift + D - spawn a die\nClick + D - delete a die\nClick + R - roll a die\nClick + 1/2/3/4/5/6 - assign value to a die");
        
        addChild(std::move(closeButton));
        addChild(std::move(controlsText));
    }

}