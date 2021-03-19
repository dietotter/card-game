#include "Controls.h"

#include "../Button.h"

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

        addChild(std::move(closeButton));
    }

}