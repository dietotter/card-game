#pragma once

namespace cnst {

    // cards in real life will be 63x88, so this size is proportionally increased
    inline constexpr int cardWidth{ 126 };
    inline constexpr int cardHeight{ 176 };

    // image width and height on card
    inline constexpr int cardImageWidth{ 120 };
    inline constexpr int cardImageHeight{ 80 };

    inline constexpr unsigned int cardBackgroundColor{ 0xCDCDCDFF };
    inline constexpr unsigned int cardTextColor{ 0x333333FF };
    inline constexpr int cardFontSize{ 16 };

    // positions on card
    inline constexpr int cardContentXOffset{ 3 };
    inline constexpr int cardNameYOffset{ 3 };
    inline constexpr int cardImageYOffset{ cardNameYOffset * 2 + cardFontSize };
    inline constexpr int cardDescriptionYOffset{ cardImageYOffset + cardImageHeight + cardNameYOffset };
    
}