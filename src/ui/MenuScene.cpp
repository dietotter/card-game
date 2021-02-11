#include "MenuScene.h"
#include "Text.h"

namespace nik {

    void MenuScene::inputUpdate(const sf::Event &event)
    {
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
        
        auto text{ std::make_unique<Text>(300, 300, "Test", 120) };
        text->setColor(sf::Color::Magenta);
        m_canvas.addChild(std::move(text));
    }

}