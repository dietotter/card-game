#include "List.h"
#include "../core/Button.h"

namespace nik {
    
    std::unique_ptr<UIElement> List::clone() const
    {
        return std::make_unique<List>(*this);
    }

    List::List(int width): Area()
    {
        setWidth(width);
        setColor(sf::Color(0, 0, 0, 200));
        setOutlineThickness(1);
        setOutlineColor(sf::Color::White);

        // turn off propagation
        onClick = [](auto event) { return true; };
    }
        
    void List::addItem(const std::string &item)
    {
        auto listItem{ std::make_unique<Button>(0, m_childrenList.size() * m_itemHeight, 0, m_itemHeight) };

        listItem->setWidthPercent(100, getWidth());
        listItem->setColor(sf::Color(0x550D08FF));
        listItem->onClick = [this, item](const sf::Event &event) {
            this->m_selectedItem = item;
            return true;
        };
        listItem->setTextString(item);
        listItem->setCharacterSize(m_itemCharacterSize);
        listItem->setTextColor(sf::Color(0xD6E0FDFF));
        listItem->setName(item);

        addChild(std::move(listItem));
        setHeight(m_childrenList.size() * m_itemHeight);
    }
    
    void List::removeItem(const std::string &item)
    {
        getChildren().erase(
            std::remove_if(
                getChildren().begin(),
                getChildren().end(),
                getUIElementNameComparator(item)
            ),
            getChildren().end()
        );

        updateItemPositions();
        setHeight(m_childrenList.size() * m_itemHeight);
    }
    
    void List::updateList(const std::vector<std::string> &items)
    {
        getChildren().clear();

        int positionCounter{ 0 };
        for (const auto &item : items)
        {
            auto listItem{ std::make_unique<Button>(0, positionCounter * m_itemHeight, 0, m_itemHeight) };

            listItem->setWidthPercent(100, getWidth());
            listItem->setColor(sf::Color(0x550D08FF));
            listItem->onClick = [this, item](const sf::Event &event) {
                this->m_selectedItem = item;
                return true;
            };
            listItem->setTextString(item);
            listItem->setCharacterSize(m_itemCharacterSize);
            listItem->setTextColor(sf::Color(0xD6E0FDFF));
            listItem->setName(item);

            addChild(std::move(listItem));

            ++positionCounter;
        }
        
        setHeight(m_childrenList.size() * m_itemHeight);
    }

    void List::setItemHeight(int height)
    {
        m_itemHeight = height;
        updateItemPositions();
        setHeight(m_childrenList.size() * m_itemHeight);
    }

    void List::setItemCharacterSize(int characterSize)
    {
        m_itemCharacterSize = characterSize;
        // TODO won't work for existing items
    }

    bool List::handleEvent(const sf::Event &event)
    {
        bool handled{ UIElement::handleEvent(event) };

        if (!handled && event.type == sf::Event::MouseButtonPressed && !contains(event.mouseButton.x, event.mouseButton.y))
        {
            m_selectedItem = "";
        }

        return handled;
    }

    void List::updateItemPositions()
    {
        int positionCounter{ 0 };
        for (auto &child : getChildren())
        {
            child->setPosition(0, positionCounter * m_itemHeight);
            ++positionCounter;
        }
    }

}
