#include "setup.h"
#include "core-constants.h"

#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <regex>

namespace nik::setup {

    // identifiers inside unnamed namespaces have internal linkage,
    // so they can't be seen from outside of this file
    namespace {

        std::vector<Card> p_library;
        sf::Texture p_cardTexture;
        sf::Texture p_cardBackTexture;
        sf::Texture p_dieTexture;
        sf::Font p_globalFont;

        void setupFont()
        {
            if (!p_globalFont.loadFromFile("fonts/cour.ttf"))
            {   
                throw std::runtime_error("Font Courier New couldn't be loaded");
            }
        }

        void loadLibraryFromFile()
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
                        p_library.push_back({ id, name, strInput });
                        mode = CardReadingMode::readId;
                        break;
                    default:
                        break;
                }
            }

            // Destructor will close the file
        }

        void createCardTexture()
        {
            // Create card text to be drawn (used for both name and description)
            sf::Text cardText;
            cardText.setFont(p_globalFont);
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
            if (!cardRenderTexture.create(cnst::cardWidth * p_library.size(), cnst::cardHeight))
            {
                throw std::runtime_error("Card render texture couldn't be created");
            }

            cardRenderTexture.clear(sf::Color(cnst::cardBackgroundColor));
            for (const auto &card : p_library)
            {
                // Draw card name
                cardText.setString(card.getName());
                cardText.setCharacterSize(cnst::cardNameSize);
                cardText.setStyle(sf::Text::Bold);
                cardText.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardNameYOffset);
                cardRenderTexture.draw(cardText);

                // Draw card image
                cardsImageSprite.setTextureRect(sf::IntRect(
                    cnst::cardImageWidth * card.getId(),
                    0,
                    cnst::cardImageWidth,
                    cnst::cardImageHeight
                ));
                cardsImageSprite.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardImageYOffset);
                cardRenderTexture.draw(cardsImageSprite);

                // Draw card description
                cardText.setString(card.getDescription());
                cardText.setCharacterSize(cnst::cardDescriptionSize);
                cardText.setStyle(sf::Text::Regular);
                cardText.setPosition(cnst::cardWidth * card.getId() + cnst::cardContentXOffset, cnst::cardDescriptionYOffset);
                cardRenderTexture.draw(cardText);
            }
            cardRenderTexture.display();
            
            p_cardTexture = cardRenderTexture.getTexture();
        }

        void createCardBackTexture()
        {
            if (!p_cardBackTexture.loadFromFile("images/cardbacks-sheet.png"))
            {
                throw std::runtime_error("Card backs image sheet couldn't be loaded");
            }
        }

        void assignCardTextures()
        {
            for (auto &card : p_library)
            {
                card.setFaceUpTexture(p_cardTexture);
                card.setFaceUpTextureRect(sf::IntRect(
                    cnst::cardWidth * card.getId(),
                    0,
                    cnst::cardWidth,
                    cnst::cardHeight
                ));

                card.setFaceDownTexture(p_cardBackTexture);
                // when entering online play, the hosting (or the connecting) player should have the card back texture rect changed
                card.setFaceDownTextureRect(sf::IntRect(
                    0,
                    0,
                    cnst::cardWidth,
                    cnst::cardHeight
                ));
            }
        }

        void createDieTexture()
        {
            // Create texture for all the dice to use
            sf::RenderTexture dieRenderTexture;
            if (!dieRenderTexture.create(cnst::dieWidth * 12, cnst::dieWidth))
            {
                throw std::runtime_error("Die render texture couldn't be created");
            }

            dieRenderTexture.clear(sf::Color(cnst::dieBackgroundColor));

            // Dice dots size and color
            sf::Text dieNumber;
            dieNumber.setFont(p_globalFont);
            dieNumber.setCharacterSize(cnst::dieWidth);
            dieNumber.setStyle(sf::Text::Bold);
            dieNumber.setFillColor(sf::Color(cnst::dieNumberColor));

            for (int i{ 0 }; i < 6; ++i)
            {
                dieNumber.setString(std::to_string(i + 1));
                dieNumber.setPosition(cnst::dieWidth * i + cnst::dieNumberOffset, -cnst::dieNumberOffset);

                dieRenderTexture.draw(dieNumber);
            }

            sf::Text rolledDiceSymbol;
            rolledDiceSymbol.setString("R");
            rolledDiceSymbol.setFont(p_globalFont);
            rolledDiceSymbol.setCharacterSize(cnst::dieWidth / 2);
            rolledDiceSymbol.setStyle(sf::Text::Bold);
            rolledDiceSymbol.setFillColor(sf::Color(cnst::dieNumberColor));
            // values from 6 to 11 are for rolled (randomized) dice
            for (int i{ 6 }; i < 12; ++i)
            {
                dieNumber.setString(std::to_string(i - 5));
                dieNumber.setPosition(cnst::dieWidth * i + cnst::dieNumberOffset / 2, -cnst::dieNumberOffset);
                rolledDiceSymbol.setPosition(cnst::dieWidth * i + cnst::dieWidth / 2 + cnst::dieNumberOffset / 2, cnst::dieNumberOffset * 2);

                dieRenderTexture.draw(dieNumber);
                dieRenderTexture.draw(rolledDiceSymbol);
            }

            dieRenderTexture.display();
            p_dieTexture = dieRenderTexture.getTexture();
        }

    }

    bool initialize()
    {
        try
        {
            setupFont();
            loadLibraryFromFile();
            createCardTexture();
            createCardBackTexture();
            assignCardTextures();
            createDieTexture();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }

        return true;
    }

    const std::vector<Card>& getLibrary() { return p_library; }
    const sf::Texture& getDieTexture() { return p_dieTexture; }
    const sf::Font& getGlobalFont() { return p_globalFont; }

    Deck loadDeckFromFile(const std::string &filename)
    {
        std::ifstream inf{ filename };

        if (!inf)
        {
            throw std::runtime_error("Couldn't open cards data file");
        }

        Deck deck;

        while (inf)
        {
            // TODO error handling on wrong input
            int cardId;
            inf >> cardId;
            
            // this assumes that card id = card's position in library
            deck.putCardOnTop(p_library[cardId]);
        }

        return deck;
    }

    Deck loadDeckFromString(const std::string &deckString)
    {
        std::istringstream ss{ deckString };

        // TODO idk if stringstream can be initialized incorrectly
        if (!ss)
        {
            throw std::runtime_error("Something wrong with deck string");
        }

        std::string token;
        Deck deck;

        while (std::getline(ss, token))
        {
            // TODO error handling on wrong input
            int cardId{ std::stoi(token) };
            
            // this assumes that card id = card's position in library
            deck.putCardOnTop(p_library[cardId]);
        }

        return deck;
    }

    std::string loadDeckStringFromFile(const std::string &filename)
    {
        std::ifstream inf{ filename };

        if (!inf)
        {
            throw std::runtime_error("Couldn't open cards data file");
        }

        std::string deckString;

        while (inf)
        {
            // TODO error handling on wrong input
            int cardId;
            inf >> cardId;
            
            deckString += std::to_string(cardId) + '\n';
        }

        return deckString;
    }

    std::string convertDeckToString(const Deck &deck)
    {
        std::string deckString;

        for (const auto &card : deck.getCardList())
        {
            deckString += std::to_string(card.getId()) + '\n';
        }

        return deckString;
    }

}