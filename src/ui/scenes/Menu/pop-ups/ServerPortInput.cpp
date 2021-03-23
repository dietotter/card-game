#include "ServerPortInput.h"

#include "../../../core/TextInput.h"
// ======= server test part =======
#include "../../../core/Text.h"
#include "../../../../net/Server.h"
// ======= server test part =======

#include <SFML/Graphics.hpp>

// ======= server test part =======
#include <string>
#include <exception>
// ======= server test part =======

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
        // ======= server test part =======
        portInput->onEnterPress = [this](const std::string &inputString) {
            if (inputString.empty())
            {
                try
                {
                    Server::listen();
                    addChild(std::make_unique<Text>(50, 0, "Server started!"));
                }
                catch (const std::exception &e)
                {
                    addChild(std::make_unique<Text>(50, 0, e.what()));
                }
            }
            else
            {
                // TODO if we don't stop server, exception happens on "Quit" button press
                Server::stop();
            }
        };
        // ======= server test part =======
        
        addChild(std::move(portInput));
    }

}