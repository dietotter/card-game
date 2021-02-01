#pragma once

#include "Card.h"
#include "Deck.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class SetupManager
{
private:
    std::vector<Card> m_library;
    sf::Texture m_cardTexture;
    sf::Texture m_cardBackTexture;

    void loadLibraryFromFile();
    void createCardTexture();
    void createCardBackTexture();
    void assignCardTextures();

public:
    SetupManager() = default;

    bool initialize();

    std::vector<Card>& getLibrary() { return m_library; }
    std::vector<Card> getLibrary() const { return m_library; }
    const sf::Texture& getCardTexture() const { return m_cardTexture; }

    Deck loadDeckFromFile(const std::string &filename);
};