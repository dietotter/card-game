#pragma once

#include "game-core/Card.h"
#include "game-core/Deck.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

namespace nik::setup
{
    bool initialize();

    const std::vector<Card>& getLibrary();
    const sf::Texture& getDieTexture();
    const sf::Font& getGlobalFont();

    Deck loadDeckFromFile(const std::string &filename);
    Deck loadDeckFromString(const std::string &deckString);
    std::string loadDeckStringFromFile(const std::string &filename);
    std::string convertDeckToString(const Deck &deck);
}
