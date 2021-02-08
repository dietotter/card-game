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

    Deck loadDeckFromFile(const std::string &filename);
}
