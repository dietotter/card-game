#pragma once

namespace nik::cnst {

    inline constexpr char deckFolderPath[]{ "data/decks" };

    inline constexpr int screenWidth{ 1920 };
    inline constexpr int screenHeight{ 1080 };

    // cards in real life will be 63x88, so this size is proportionally increased 4 times
    inline constexpr int cardWidth{ 252 };
    inline constexpr int cardHeight{ 352 };

    // image width and height on card
    inline constexpr int cardImageWidth{ 240 };
    inline constexpr int cardImageHeight{ 160 };

    inline constexpr unsigned int cardBackgroundColor{ 0xCDCDCDFF };
    inline constexpr unsigned int cardTextColor{ 0x333333FF };
    inline constexpr int cardNameSize{ 16 };
    inline constexpr int cardDescriptionSize{ 13 };

    // positions on card
    inline constexpr int cardContentXOffset{ 6 };
    inline constexpr int cardNameYOffset{ 6 };
    inline constexpr int cardImageYOffset{ cardNameYOffset * 2 + cardNameSize };
    inline constexpr int cardDescriptionYOffset{ cardImageYOffset + cardImageHeight + cardNameYOffset };
    
    inline constexpr unsigned int boardBackgroundColor{ 0x3D4849FF };

    // dice info
    inline constexpr int dieWidth{ 50 };
    inline constexpr int dieNumberOffset{ dieWidth / 5 };
    inline constexpr unsigned int dieNumberColor{ 0x333333FF };
    inline constexpr unsigned int dieBackgroundColor{ 0xFFFFDDFF };
}