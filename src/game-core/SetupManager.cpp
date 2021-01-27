#include "SetupManager.h"
#include "core-constants.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <regex>

void SetupManager::loadLibraryFromFile()
{
    enum CardReadingMode
    {
        readId, readName, readDescription
    };

    std::ifstream inf{ "data/cards.dat" };

    if (!inf)
    {
        throw std::runtime_error("Couldn't open cards data file");
    }

    auto mode{ CardReadingMode::readId };

    int id;
    std::string name;

    while (inf)
    {
        std::string strInput;
        std::getline(inf, strInput);

        // if the line is empty or commented, ignore it
        if (strInput.empty() || strInput[0] == '#')
        {
            continue;
        }
        
        switch(mode)
        {
            case CardReadingMode::readId:
                id = std::stoi(strInput);
                mode = CardReadingMode::readName;
                break;
            case CardReadingMode::readName:
                name = strInput;
                mode = CardReadingMode::readDescription;
                break;
            case CardReadingMode::readDescription:
                strInput = std::regex_replace(strInput, std::regex("\\\\n"), "\n");
                m_library.push_back({ id, name, strInput });
                mode = CardReadingMode::readId;
                break;
            default:
                break;
        }
    }

    // Destructor will close the file
}

void SetupManager::createCardTexture()
{
    // TODO move loading font to separate setup function (maybe even separate setup manager)
    sf::Font font;
    if (!font.loadFromFile("fonts/cour.ttf"))
    {   
        throw std::runtime_error("Font Courier New couldn't be loaded");
    }

    // Create card text to be drawn (used for both name and description)
    sf::Text cardText;
    cardText.setFont(font);
    cardText.setCharacterSize(cnst::cardFontSize);
    cardText.setFillColor(sf::Color(cnst::cardTextColor));

    // Load card images and set up sprite for drawing corresponding image onto each card
    sf::Texture cardsImageSheet;
    if (!cardsImageSheet.loadFromFile("images/cards-sheet.png"))
    {   
        throw std::runtime_error("Cards image sheet couldn't be loaded");
    }

    sf::Sprite cardsImageSprite;
    cardsImageSprite.setTexture(cardsImageSheet);

    // Create a single cards texture for each card to use
    sf::RenderTexture cardRenderTexture;
    if (!cardRenderTexture.create(cnst::cardWidth * m_library.size(), cnst::cardHeight))
    {
        throw std::runtime_error("Card render texture couldn't be created");
    }

    cardRenderTexture.clear(sf::Color(cnst::cardBackgroundColor));
    for (auto &card : m_library)
    {
        cardText.setString(card.getName());
        cardText.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardNameYOffset);
        cardRenderTexture.draw(cardText);

        cardsImageSprite.setTextureRect(sf::IntRect(
            cnst::cardImageWidth * card.getId(),
            0,
            cnst::cardImageWidth,
            cnst::cardImageHeight
        ));
        cardsImageSprite.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardImageYOffset);
        cardRenderTexture.draw(cardsImageSprite);

        cardText.setString(card.getDescription());
        cardText.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardDescriptionYOffset);
        cardRenderTexture.draw(cardText);

    }
    cardRenderTexture.display();
    
    m_cardTexture = cardRenderTexture.getTexture();
}