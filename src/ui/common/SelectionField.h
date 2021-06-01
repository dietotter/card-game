#pragma once

#include "../core/TextInput.h"

#include <vector>

namespace nik {
    
    class SelectionField : public TextInput
    {
    private:
        void initializeList();

    protected:
        virtual std::unique_ptr<UIElement> clone() const override;

    public:
        SelectionField();
        SelectionField(int x, int y, float width = 200, float height = 50);
        SelectionField(const SelectionField &selectionField);

        void setOptions(const std::vector<std::string> &items);

        virtual bool handleEvent(const sf::Event &event) override;
    };

}
