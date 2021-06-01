#include "SelectionField.h"
#include "List.h"

namespace nik {
    // if no Base contructor is specified, default constructor will be used
    SelectionField::SelectionField()
    {
        initializeList();
    }

    SelectionField::SelectionField(int x, int y, float width, float height): TextInput{ x, y, width, height }
    {
        initializeList();
    }

    SelectionField::SelectionField(const SelectionField &selectionField): TextInput{ selectionField }
    {
        initializeList();
    }

    void SelectionField::setOptions(const std::vector<std::string> &items)
    {
        auto optionsList{ dynamic_cast<List*>(m_childrenList.front().get()) };
        
        if (optionsList)
        {
            optionsList->updateList(items);
        }
    }

    bool SelectionField::handleEvent(const sf::Event &event)
    {
        if (m_hidden)
        {
            return false;
        }

        for (auto it{ m_childrenList.rbegin() }; it != m_childrenList.rend(); ++it)
        {
            bool handled{ (*it)->handleEvent(event) };
            if (handled)
            {
                // if the first element (options list) handled the event, update the new field value and hide the list
                // (sf::Event::MouseButtonReleased because List uses Buttons as items)
                if (it + 1 == m_childrenList.rend() && event.type == sf::Event::MouseButtonReleased)
                {  
                    auto optionsList{ dynamic_cast<List*>((*it).get()) };

                    if (optionsList)
                    {
                        setString(optionsList->getSelectedItem());
                        optionsList->setHidden(true);
                        setFocused(false);
                    }
                }

                return true;
            }
        }
        
        if (event.type == sf::Event::MouseButtonPressed)
        {
            auto optionsList{ dynamic_cast<List*>(m_childrenList.front().get()) };
            if (contains(event.mouseButton.x, event.mouseButton.y))
            {
                if (!isFocused())
                {
                    setFocused();
                    if (optionsList)
                    {
                        optionsList->setHidden(false);
                    }
                }
            
                return onClick(event);
            }
            else if (!contains(event.mouseButton.x, event.mouseButton.y) && isFocused())
            {
                setFocused(false);
                if (optionsList)
                {
                    optionsList->setHidden(true);
                }
            }
        }

        return false;
    }

    std::unique_ptr<UIElement> SelectionField::clone() const
    {
        return std::make_unique<SelectionField>(*this);
    }

    void SelectionField::initializeList()
    {
        auto listElement{ std::make_unique<List>(getWidth()) };
        listElement->setPosition(0, getHeight());
        listElement->setHidden(true);

        addChild(std::move(listElement));
    }
    
}
