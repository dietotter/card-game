#include "LobbyScene.h"

namespace nik {

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

    void LobbyScene::initialize()
    {
        sf::Vector2f windowSize{ m_window.getSize() };
        m_canvas.setPercentSize(100, 100, windowSize.x, windowSize.y);
        m_canvas.setColor(sf::Color(0x281903FF));
    }

    std::unique_ptr<Scene> LobbyScene::clone() const
    {
        return std::make_unique<LobbyScene>(*this);
    }
    
}