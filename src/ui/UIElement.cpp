#include "UIElement.h"

namespace nik {

    void UIElement::deepCopy(const UIElement &element)
    {
        m_width = element.m_width;
        m_height = element.m_height;
        m_widthPercent = element.m_widthPercent;
        m_heightPercent = element.m_heightPercent;

        m_childrenList.resize(element.m_childrenList.size());
        for (int i{ 0 }; i < m_childrenList.size(); ++i)
        {
            m_childrenList[i] = element.m_childrenList[i]->clone();
        }
    }

    void UIElement::adjustWidth(float parentWidth)
    {
        m_width = parentWidth / 100 * m_widthPercent; 
    }

    void UIElement::adjustHeight(float parentHeight)
    {
        m_height = parentHeight / 100 * m_heightPercent;
    }

    void UIElement::adjustChildrenSize()
    {
        for (auto &element : m_childrenList)
        {
            element->adjustSize(m_width, m_height);
        }
    }

    void UIElement::adjustSize(float parentWidth, float parentHeight)
    {
        if (m_widthPercent)
        {
            adjustWidth(parentWidth);
        }

        if (m_heightPercent)
        {
            adjustHeight(parentHeight);
        }

        adjustDrawingSize();
        adjustChildrenSize();
    }

    UIElement::UIElement(int x, int y, float width, float height)
        : m_width{ width }, m_height{ height }
    {
        setPosition(x, y);
    }

    UIElement::UIElement(const UIElement &element)
    {
        deepCopy(element);
    }

    UIElement& UIElement::operator=(const UIElement &element)
    {
        if (&element != this)
        {
            deepCopy(element);
        }

        return *this;
    }

    void UIElement::setWidth(float width)
    {
        // make width no longer depend on percentage
        m_widthPercent = 0;
        m_width = width;

        adjustDrawingSize();
        adjustChildrenSize();
    }

    void UIElement::setHeight(float height)
    {
        // make height no longer depend on percentage
        m_heightPercent = 0;
        m_height = height;

        adjustDrawingSize();
        adjustChildrenSize();
    }

    void UIElement::setWidthPercent(int percent, float parentWidth)
    {
        m_widthPercent = percent;
        adjustWidth(parentWidth);

        adjustDrawingSize();
        adjustChildrenSize();
    }

    void UIElement::setHeightPercent(int percent, float parentHeight)
    {
        m_heightPercent = percent;
        adjustHeight(parentHeight);

        adjustDrawingSize();
        adjustChildrenSize();
    }

    void UIElement::setPercentSize(int widthPercent, int heightPercent, float parentWidth, float parentHeight)
    {
        m_widthPercent = widthPercent;
        m_heightPercent = heightPercent;

        adjustSize(parentWidth, parentHeight);
    }

    sf::FloatRect UIElement::getBoundingBox() const
    {
        return { getPosition(), { m_width, m_height } };
    }
    
    bool UIElement::contains(int x, int y) const
    {
        return getBoundingBox().contains(x, y);
    }
    
    bool UIElement::handleEvent(const sf::Event &event)
    {
        for (const auto &child : m_childrenList)
        {
            bool handled{ child->handleEvent(event) };
            if (handled)
            {
                return true;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed && contains(event.mouseButton.x, event.mouseButton.y))
        {
            return onClick(event);
        }

        return false;
    }

}