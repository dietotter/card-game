#pragma once

#include "Card.h"

#include <SFML/Graphics.hpp>

#include <vector>

class SetupManager
{
private:
    std::vector<Card> m_library;
    sf::Texture m_cardTexture;

public:
    SetupManager() = default;

    void loadLibraryFromFile();
    void createCardTexture();
    void createCardBackTexture();

    const std::vector<Card>& getLibrary() const { return m_library; }
    const sf::Texture& getCardTexture() const { return m_cardTexture; }
};