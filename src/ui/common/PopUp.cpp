#include "PopUp.h"

#include "../core/Button.h"

namespace nik {

    std::unique_ptr<UIElement> PopUp::clone() const
    {
        return std::make_unique<PopUp>(*this);
    }

    PopUp::PopUp(): Area()
    {
        setColor(sf::Color(0, 0, 0, 200));

        // turn off propagation
        onClick = [](auto event) { return true; };
        
        auto closeButton{ std::make_unique<Button>(0, 0, 50, 50) };
        closeButton->setColor(sf::Color::Transparent);
        closeButton->onClick = [this](const sf::Event &event) {
            this->setHidden(true);
            return true;
        };
        closeButton->setTextString("x");
        closeButton->setTextStyle(sf::Text::Bold);
        closeButton->setCharacterSize(40);
        closeButton->setTextColor(sf::Color::White);
        closeButton->setTextPressedColor(sf::Color(200, 200, 200, 255));
        closeButton->setName("PopUpCloseButton");
        
        addChild(std::move(closeButton));
    }

    bool PopUp::handleEvent(const sf::Event &event)
    {
        bool handled{ UIElement::handleEvent(event) };

        if (!handled && event.type == sf::Event::MouseButtonPressed && !contains(event.mouseButton.x, event.mouseButton.y))
        {
            setHidden(true);
        }

        return handled;
    }

}