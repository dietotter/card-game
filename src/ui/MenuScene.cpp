#include "MenuScene.h"
#include "Text.h"
#include "Button.h"
#include "components/Controls.h"

#include <algorithm>
#include <memory>
#include <string>

namespace nik {

    void MenuScene::inputUpdate(const sf::Event &event)
    {
        if (event.type == sf::Event::Resized)
        {
            // update the canvas size to the new size of the window (set in Scene class)
            sf::Vector2f windowSize{ m_window.getSize() };
            m_canvas.setPercentSize(m_canvas.getWidthPercent(), m_canvas.getHeightPercent(), windowSize.x, windowSize.y);
        }

        m_canvas.handleEvent(event);
    }

    void MenuScene::graphicsUpdate()
    {
        m_window.draw(m_canvas);
    }

    void MenuScene::initialize()
    {
        sf::Vector2f windowSize{ m_window.getSize() };
        m_canvas.setPercentSize(100, 100, windowSize.x, windowSize.y);
        m_canvas.setColor(sf::Color(0x281903FF));
        m_canvas.onClick = [this](const sf::Event &event) {
            // temporary solution for pop-up click-off hiding
            auto controlsElement{ std::find_if(
                this->m_canvas.getChildren().begin(),
                this->m_canvas.getChildren().end(),
                getUIElementNameComparator("ControlsPopUp")
            ) };
            controlsElement->get()->setHidden(true);
            return false;
        };

        auto title{ std::make_unique<Text>("Nik's Card Game Simulator Thing") };
        title->setPercentPosition(10, 10, m_canvas.getWidth(), m_canvas.getHeight());
        title->setCharacterSize(50);
        title->setStyle(sf::Text::Bold);
        title->setColor(sf::Color(0xD6E0FDFF));

        auto startButton { std::make_unique<Button>() };
        auto playOnlineButton { std::make_unique<Button>() };
        auto collectionButton { std::make_unique<Button>() };
        auto controlsButton { std::make_unique<Button>() };
        auto quitButton { std::make_unique<Button>() };

        startButton->setWidthPercent(80, m_canvas.getWidth());
        startButton->setPercentPosition(10, 25, m_canvas.getWidth(), m_canvas.getHeight());
        startButton->setHeight(100.f);
        startButton->setColor(sf::Color(0x550D08FF));
        startButton->setOutlineThickness(2);
        startButton->setOutlineColor(sf::Color(0x3E1507FF));
        startButton->onClick = [this](const sf::Event &event) {
            this->m_requestScene("Game");
            return true;
        };
        startButton->setTextString("Play");
        startButton->setCharacterSize(40);
        startButton->setTextColor(sf::Color(0xD6E0FDFF));
        startButton->setTextPressedColor(sf::Color(0x929B22FF));
        startButton->setTextStyle(sf::Text::Bold);

        auto controlsPopUp{ std::make_unique<Controls>() };
        controlsPopUp->setPercentSize(90, 90, m_canvas.getWidth(), m_canvas.getHeight());
        controlsPopUp->setPercentPosition(5, 5, m_canvas.getWidth(), m_canvas.getHeight());
        controlsPopUp->setName("ControlsPopUp");
        controlsPopUp->setHidden(true);

        controlsButton->setWidthPercent(80, m_canvas.getWidth());
        controlsButton->setPercentPosition(10, 40, m_canvas.getWidth(), m_canvas.getHeight());
        controlsButton->setHeight(100.f);
        controlsButton->setColor(sf::Color(0xB5DAF3FF));
        controlsButton->setOutlineThickness(2);
        controlsButton->setOutlineColor(sf::Color(0x0F0E00FF));
        controlsButton->onClick = [this](const sf::Event &event) {
            auto controlsElement{ std::find_if(
                this->m_canvas.getChildren().begin(),
                this->m_canvas.getChildren().end(),
                getUIElementNameComparator("ControlsPopUp")
                // TODO hide pop-up on click-off, but also don't stop propagating if done so
                // (will also be useful for in-game pop-ups)
            ) };
            controlsElement->get()->setHidden(false);
            return true;
        };
        controlsButton->setTextString("Controls");
        controlsButton->setCharacterSize(40);
        controlsButton->setTextColor(sf::Color(0x3E1507FF));
        controlsButton->setTextPressedColor(sf::Color(0x6E350FFF));
        controlsButton->setTextStyle(sf::Text::Bold);

        playOnlineButton->setWidthPercent(80, m_canvas.getWidth());
        playOnlineButton->setPercentPosition(10, 55, m_canvas.getWidth(), m_canvas.getHeight());
        playOnlineButton->setHeight(100.f);
        playOnlineButton->setColor(sf::Color(0x550D08FF));
        playOnlineButton->setOutlineThickness(2);
        playOnlineButton->setOutlineColor(sf::Color(0x3E1507FF));
        playOnlineButton->onClick = [this](const sf::Event &event) {
            return true;
        };
        playOnlineButton->setTextString("Play online");
        playOnlineButton->setCharacterSize(40);
        playOnlineButton->setTextColor(sf::Color(0xD6E0FDFF));
        playOnlineButton->setTextPressedColor(sf::Color(0x929B22FF));
        playOnlineButton->setTextStyle(sf::Text::Bold);

        collectionButton->setWidthPercent(80, m_canvas.getWidth());
        collectionButton->setPercentPosition(10, 70, m_canvas.getWidth(), m_canvas.getHeight());
        collectionButton->setHeight(100.f);
        collectionButton->setColor(sf::Color(0xB5DAF3FF));
        collectionButton->setOutlineThickness(2);
        collectionButton->setOutlineColor(sf::Color(0x0F0E00FF));
        collectionButton->onClick = [this](const sf::Event &event) {
            return true;
        };
        collectionButton->setTextString("Collection");
        collectionButton->setCharacterSize(40);
        collectionButton->setTextColor(sf::Color(0x3E1507FF));
        collectionButton->setTextPressedColor(sf::Color(0x6E350FFF));
        collectionButton->setTextStyle(sf::Text::Bold);

        quitButton->setWidthPercent(80, m_canvas.getWidth());
        quitButton->setPercentPosition(10, 85, m_canvas.getWidth(), m_canvas.getHeight());
        quitButton->setHeight(100.f);
        quitButton->setColor(sf::Color(0x550D08FF));
        quitButton->setOutlineThickness(2);
        quitButton->setOutlineColor(sf::Color(0x3E1507FF));
        quitButton->onClick = [this](const sf::Event &event) {
            this->m_window.close();
            return true;
        };
        quitButton->setTextString("Quit");
        quitButton->setCharacterSize(40);
        quitButton->setTextColor(sf::Color(0xD6E0FDFF));
        quitButton->setTextPressedColor(sf::Color(0x929B22FF));
        quitButton->setTextStyle(sf::Text::Bold);

        m_canvas.addChild(std::move(title));
        m_canvas.addChild(std::move(startButton));
        m_canvas.addChild(std::move(controlsButton));
        m_canvas.addChild(std::move(collectionButton));
        m_canvas.addChild(std::move(playOnlineButton));
        m_canvas.addChild(std::move(quitButton));
        m_canvas.addChild(std::move(controlsPopUp));
    }

    std::unique_ptr<Scene> MenuScene::clone() const
    {
        return std::make_unique<MenuScene>(*this);
    }

}