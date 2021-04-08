#include "LobbyScene.h"
#include "../../core/Button.h"
#include "../../core/Text.h"
#include "../../../net/Client.h"
#include "../../../net/Server.h"
#include "../../../net/events/ConnectionEvent.h"
#include "../../../net/events/LobbyEvent.h"

#include <string>

namespace nik {

    void LobbyScene::clientEventUpdate(NetworkEvent *event)
    {
        if (event->getType() == NetworkEvent::Type::lobbyUpdatePlayersList)
        {
            auto *playersListEvent{ dynamic_cast<UpdatePlayersListEvent*>(event) };
            if (playersListEvent)
            {
                auto playersIdList{ playersListEvent->getPlayersIdList() };
                int i{ 1 };
                for (auto id : playersIdList)
                {
                    std::string playerLabel{ "Player " + std::to_string(id) };
                    if (id == Client::getIdOnServer())
                    {
                        playerLabel += " (you)";
                    }
                    m_canvas.addChild(std::make_unique<Text>(200, 300 + i * 100, playerLabel));
                    ++i;
                }
            }
        }
    }

    void LobbyScene::serverEventUpdate(NetworkEvent *event)
    {
        if (event->getType() == NetworkEvent::Type::clientConnected)
        {
            auto *connectedEvent{ dynamic_cast<ClientConnectedEvent*>(event) };
            if (connectedEvent)
            {
                m_players.push_back(Player(connectedEvent->getClientId()));
                Server::sendEvent(std::make_unique<UpdatePlayersListEvent>(m_players));
                m_canvas.addChild(std::make_unique<Text>(200, 300 + m_players.size() * 100, "Player " + std::to_string(connectedEvent->getClientId())));
            }
        }
    }

    void LobbyScene::inputUpdate(const sf::Event &event)
    {
        if (event.type == sf::Event::Resized)
        {
            // update the canvas size to the new size of the window (set in Scene class)
            sf::Vector2f windowSize{ m_window.getSize() };
            // TODO if element has been set a percentage-based location, it is not adjusted right now
            m_canvas.setPercentSize(m_canvas.getWidthPercent(), m_canvas.getHeightPercent(), windowSize.x, windowSize.y);
        }

        m_canvas.handleEvent(event);
    }

    void LobbyScene::graphicsUpdate()
    {
        m_window.draw(m_canvas);
    }

    void LobbyScene::initialize(const std::string &params)
    {
        sf::Vector2f windowSize{ m_window.getSize() };
        m_canvas.setPercentSize(100, 100, windowSize.x, windowSize.y);
        m_canvas.setColor(sf::Color(0x281903FF));

        auto quitButton { std::make_unique<Button>(50, 50, 700, 100) };
        quitButton->setColor(sf::Color(0x550D08FF));
        quitButton->setOutlineThickness(2);
        quitButton->setOutlineColor(sf::Color(0x3E1507FF));
        quitButton->setTextString("Quit lobby");
        quitButton->setCharacterSize(40);
        quitButton->setTextColor(sf::Color(0xD6E0FDFF));
        quitButton->setTextPressedColor(sf::Color(0x929B22FF));
        quitButton->setTextStyle(sf::Text::Bold);

        m_role = static_cast<Role>(std::stoi(params));
        if (m_role == Role::server)
        {
            m_canvas.addChild(std::make_unique<Text>(200, 300, "Player Server (you)"));

            quitButton->onClick = [this](const sf::Event &event) {
                // TODO send some server closed event to connected clients
                Server::stop();
                this->m_requestScene("Menu", "");
                return true;
            };
        }
        else if (m_role == Role::client)
        {
            m_canvas.addChild(std::make_unique<Text>(200, 300, "Player Server"));

            quitButton->onClick = [this](const sf::Event &event) {
                Client::disconnect();
                this->m_requestScene("Menu", "");
                return true;
            };
        }

        m_canvas.addChild(std::move(quitButton));
    }

    std::unique_ptr<Scene> LobbyScene::clone() const
    {
        return std::make_unique<LobbyScene>(*this);
    }
    
}