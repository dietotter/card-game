#pragma once

#include "../core/Area.h"

#include <string>
#include <vector>

namespace nik {

    // TODO maybe use list in SelectInput (thus elements of the list need to be clickable/selectable)
    class List : public Area
    {
    private:
        std::string m_selectedItem;

        int m_itemHeight{ 50 };
        int m_itemCharacterSize{ 32 };

        void updateItemPositions();
        
    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        List(int width = 200);

        const std::string& getSelectedItem() const { return m_selectedItem; }
        
        void addItem(const std::string &item);
        void removeItem(const std::string &item);
        void updateList(const std::vector<std::string> &items);

        void setItemHeight(int height);
        void setItemCharacterSize(int characterSize);

        virtual bool handleEvent(const sf::Event &event) override;
    };

}