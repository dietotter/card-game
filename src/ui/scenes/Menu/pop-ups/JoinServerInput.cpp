#include "JoinServerInput.h"

#include "../../Lobby/LobbyScene.h"
#include "../../../core/TextInput.h"
#include "../../../core/Text.h"
#include "../../../../net/Client.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <exception>

namespace nik {

    std::unique_ptr<UIElement> JoinServerInput::clone() const
    {
        return std::make_unique<JoinServerInput>(*this);
    }

    JoinServerInput::JoinServerInput(Scene::RequestSceneFunction requestScene): PopUp()
    {
        auto urlInput{ std::make_unique<TextInput>(50, 100, 250, 50) };
        urlInput->setCharacterSize(40);
        urlInput->setPlaceholderString("Server URL");
        urlInput->setWidthPercent(90, getWidth());
        // try to start server with entered port, when Enter is pressed on port input
        urlInput->onEnterPress = [this, requestScene](const std::string &inputString) {
            // remove previous exception message, if such existed
            std::remove_if(
                m_childrenList.begin(),
                m_childrenList.end(),
                getUIElementNameComparator("JoinServerException")
            );

            try
            {
                if (inputString.empty())
                {
                    Client::connectToServer();
                }
                else
                {
                    Client::connectToServer(inputString);
                }

                requestScene("Lobby", std::to_string(static_cast<int>(NetworkScene::Role::client)));
            }
            catch (const std::exception &e)
            {
                auto joinExceptionText{ std::make_unique<Text>(50, 0, e.what()) };
                joinExceptionText->setName("JoinServerException");
                addChild(std::move(joinExceptionText));
            }
        };
        
        addChild(std::move(urlInput));
    }

}