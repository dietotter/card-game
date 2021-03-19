#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <memory>
#include <vector>
#include <string>

namespace nik {

    class UIElement : public sf::Drawable, public sf::Transformable
    {
        using ChildrenList = std::vector<std::unique_ptr<UIElement>>;
    private:
        std::string m_name;

        void deepCopy(const UIElement &element);
        void adjustWidth(float parentWidth);
        void adjustHeight(float parentHeight);
        void adjustChildrenSize();
        void adjustSize(float parentWidth, float parentHeight);

        void setParentActualPosition(const sf::Vector2f &position);

    protected:
        ChildrenList m_childrenList;

        int m_widthPercent{};
        int m_heightPercent{};
        float m_width{};
        float m_height{};

        bool m_hidden{};

        // parent's position (actual position, considering all of the ancestor's positions)
        sf::Vector2f m_parentActualPosition{};

        // for inner SFML objects that derived classes might be using
        virtual void adjustDrawingSize() = 0;
        // virtual function for deep copying the polymorphic UIElements
        virtual std::unique_ptr<UIElement> clone() const = 0;

    public:
        UIElement(int x = 0, int y = 0, float width = 0, float height = 0, const std::string &name = "DefaultElement");
        UIElement(const UIElement &element);
        
        virtual ~UIElement() {}
        
        virtual UIElement& operator=(const UIElement &element);

        // unsure if this is correct - should this be public? should this be initialized? etc
        std::function<bool(const sf::Event&)> onClick{ [](auto event) { return false; } };
    
        ChildrenList& getChildren() { return m_childrenList; };
        const ChildrenList& getChildren() const { return m_childrenList; };
        void addChild(std::unique_ptr<UIElement> element);

        float getWidth() const { return m_width; }
        float getHeight() const { return m_height; }
        float getWidthPercent() const { return m_widthPercent; }
        float getHeightPercent() const { return m_heightPercent; }
        void setWidth(float width);
        void setHeight(float height);
        void setWidthPercent(int percent, float parentWidth);
        void setHeightPercent(int percent, float parentHeight);
        void setPercentSize(int widthPercent, int heightPercent, float parentWidth, float parentHeight);

        sf::Vector2f getActualPosition() const;
        void setPosition(float x, float y);
        void setPercentPosition(int xPercent, int yPercent, float parentWidth, float parentHeight);

        bool isHidden() const { return m_hidden; }
        void setHidden(bool hidden) { m_hidden = hidden; }

        const std::string& getName() const { return m_name; }
        void setName(const std::string &name) { m_name = name; }

        virtual void setColor(const sf::Color &color) = 0;
        virtual sf::FloatRect getBoundingBox() const;
        virtual bool contains(int x, int y) const;
        virtual bool handleEvent(const sf::Event &event);
    };

    std::function<bool(const std::unique_ptr<UIElement> &elem)> getUIElementNameComparator(const std::string &name);
}