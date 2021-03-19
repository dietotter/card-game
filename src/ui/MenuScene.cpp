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
        m_canvas.setColor(sf::Color::Cyan);
        m_canvas.onClick = [this](const sf::Event &event) {
            this->m_canvas.setColor(sf::Color(event.mouseButton.x * event.mouseButton.y));
            return true;
        };
        
        auto text{ std::make_unique<Text>(100, 200, "U ar gay", 120) };
        text->setColor(sf::Color::Magenta);
        m_canvas.addChild(std::move(text));

        auto startButton { std::make_unique<Button>(300, 100) };
        auto controlsButton { std::make_unique<Button>(300, 300) };
        auto quitButton { std::make_unique<Button>(300, 500) };

        startButton->setPercentSize(50, 10, m_canvas.getWidth(), m_canvas.getHeight());
        startButton->setColor(sf::Color::Blue);
        startButton->setOutlineThickness(2);
        startButton->setOutlineColor(sf::Color::Red);
        startButton->onClick = [this](const sf::Event &event) {
            this->m_requestScene("Game");
            return true;
        };
        startButton->setTextString("Why ar u gay?");
        startButton->setCharacterSize(35);
        startButton->setTextColor(sf::Color(255, 255, 0, 255));
        startButton->setTextPressedColor(sf::Color(255, 0, 255, 255));

        auto controlsPopUp{ std::make_unique<Controls>() };
        controlsPopUp->setPercentSize(90, 90, m_canvas.getWidth(), m_canvas.getHeight());
        controlsPopUp->setPercentPosition(5, 5, m_canvas.getWidth(), m_canvas.getHeight());
        controlsPopUp->setName("ControlsPopUp");
        controlsPopUp->setHidden(true);

        controlsButton->setWidthPercent(70, m_canvas.getWidth());
        controlsButton->setHeight(100.f);
        controlsButton->setColor(sf::Color(200, 200, 200, 255));
        controlsButton->setOutlineThickness(2);
        controlsButton->setOutlineColor(sf::Color(100, 100, 100, 255));
        controlsButton->onClick = [this](const sf::Event &event) {
            auto controlsElement{ std::find_if(
                this->m_canvas.getChildren().begin(),
                this->m_canvas.getChildren().end(),
                getUIElementNameComparator("ControlsPopUp")
            ) };
            controlsElement->get()->setHidden(false);
            return true;
        };
        controlsButton->setTextString("Controls");
        controlsButton->setCharacterSize(40);
        controlsButton->setTextColor(sf::Color(50, 50, 50, 255));
        controlsButton->setTextPressedColor(sf::Color(70, 70, 70, 255));

        quitButton->setWidthPercent(70, m_canvas.getWidth());
        quitButton->setHeight(100.f);
        quitButton->setColor(sf::Color::Red);
        quitButton->setOutlineThickness(2);
        quitButton->setOutlineColor(sf::Color::Blue);
        quitButton->onClick = [this](const sf::Event &event) {
            this->m_window.close();
            return true;
        };
        quitButton->setTextString("TERMINATE!!1aaaaaaaaaaabvvvvvbvbvbvb");
        quitButton->setCharacterSize(40);
        quitButton->setTextColor(sf::Color::White);
        quitButton->setTextPressedColor(sf::Color(200, 200, 200, 255));

        m_canvas.addChild(std::move(startButton));
        m_canvas.addChild(std::move(controlsButton));
        m_canvas.addChild(std::move(quitButton));
        m_canvas.addChild(std::move(controlsPopUp));
    }

    std::unique_ptr<Scene> MenuScene::clone() const
    {
        return std::make_unique<MenuScene>(*this);
    }

}