#include "ServerPortInput.h"

#include "../TextInput.h"

#include <SFML/Graphics.hpp>

namespace nik {

    std::unique_ptr<UIElement> ServerPortInput::clone() const
    {
        return std::make_unique<ServerPortInput>(*this);
    }

    ServerPortInput::ServerPortInput(): PopUp()
    {
        auto portInput{ std::make_unique<TextInput>(50, 100, 250, 50) };
        portInput->setCharacterSize(40);
        portInput->setPlaceholderString("Port to start the server");
        portInput->setWidthPercent(90, getWidth());
        
        addChild(std::move(portInput));
    }

}