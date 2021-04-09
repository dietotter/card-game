#include "ServerPortInput.h"

#include "../../Lobby/LobbyScene.h"
#include "../../../core/TextInput.h"
#include "../../../core/Text.h"
#include "../../../../net/Server.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <exception>

namespace nik {

    std::unique_ptr<UIElement> ServerPortInput::clone() const
    {
        return std::make_unique<ServerPortInput>(*this);
    }

    ServerPortInput::ServerPortInput(Scene::RequestSceneFunction requestScene): PopUp()
    {
        auto portInput{ std::make_unique<TextInput>(50, 100, 250, 50) };
        portInput->setCharacterSize(40);
        portInput->setPlaceholderString("Port to start the server");
        portInput->setWidthPercent(90, getWidth());
        // try to start server with entered port, when Enter is pressed on port input
        portInput->onEnterPress = [this, requestScene](const std::string &inputString) {
            // remove previous exception message, if such existed
            m_childrenList.erase(
                std::remove_if(
                    m_childrenList.begin(),
                    m_childrenList.end(),
                    getUIElementNameComparator("ServerStartException")
                ),
                m_childrenList.end()
            );

            try
            {
                if (inputString.empty())
                {
                    Server::listen();
                }
                else
                {
                    int port{ std::stoi(inputString) };
                    Server::listen(port);
                }

                requestScene("Lobby", std::to_string(static_cast<int>(NetworkScene::Role::server)));
            }
            catch (const std::exception &e)
            {
                auto startExceptionText{ std::make_unique<Text>(50, 0, e.what()) };
                startExceptionText->setName("ServerStartException");
                addChild(std::move(startExceptionText));
            }
        };
        
        addChild(std::move(portInput));
    }

}