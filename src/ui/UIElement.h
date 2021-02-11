#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace nik {

    class UIElement : public sf::Drawable, public sf::Transformable
    {
        using ChildrenList = std::vector<std::unique_ptr<UIElement>>;
    private:
        void deepCopy(const UIElement &element);
        void adjustWidth(float parentWidth);
        void adjustHeight(float parentHeight);
        void adjustChildrenSize();
        void adjustSize(float parentWidth, float parentHeight);

    protected:
        ChildrenList m_childrenList;

        int m_widthPercent{};
        int m_heightPercent{};
        float m_width{};
        float m_height{};

        // for inner SFML objects that derived classes might be using
        virtual void adjustDrawingSize() = 0;
        // virtual function for deep copying the polymorphic UIElements
        virtual std::unique_ptr<UIElement> clone() const = 0;

    public:
        UIElement(int x = 0, int y = 0, float width = 0, float height = 0);
        UIElement(const UIElement &element);
        
        virtual ~UIElement() {}
        
        virtual UIElement& operator=(const UIElement &element);

        // unsure if this is correct - should this be public? should this be initialized? etc
        std::function<bool(const sf::Event&)> onClick{ [](auto event) { return false; } };
    
        ChildrenList& getChildren() { return m_childrenList; };
        const ChildrenList& getChildren() const { return m_childrenList; };
        void addChild(std::unique_ptr<UIElement> element) { m_childrenList.push_back(std::move(element)); }

        float getWidth() { return m_width; }
        float getHeight() { return m_height; }
        void setWidth(float width);
        void setHeight(float height);
        float getWidthPercent() { return m_widthPercent; }
        float getHeightPercent() { return m_heightPercent; }
        void setWidthPercent(int percent, float parentWidth);
        void setHeightPercent(int percent, float parentHeight);
        void setPercentSize(int widthPercent, int heightPercent, float parentWidth, float parentHeight);

        virtual void setColor(const sf::Color &color) = 0;
        virtual sf::FloatRect getBoundingBox() const;
        virtual bool contains(int x, int y) const;
        virtual bool handleEvent(const sf::Event &event);
    };

}