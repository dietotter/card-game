#include "Board.h"
#include "Card.h"
#include "Deck.h"
#include "Die.h"
#include "Hand.h"
#include "game-constants.h"
#include "../core-constants.h"
#include "../setup.h"
#include "../net/network-constants.h"
#include "../net/Client.h"
#include "../net/Server.h"

#include <algorithm>

namespace nik {

    Board::Board(const Board &board)
    {
        deepCopy(board);
    }

    bool Board::handleEvent(const sf::Event &event, Role role)
    {
        // TODO implement special case for offline role (when I know how exactly I wanna do it)
        int playerId{ role == Role::client ? Client::getIdOnServer() : cnst::serverDefaultId };

        if (event.type == sf::Event::MouseButtonPressed)
        {
            // not using reverse iterator because of the std::rotate
            for (auto it{ --m_objectList.end() }; it >= m_objectList.begin(); --it)
            {
                if ((*it)->contains(event.mouseButton.x, event.mouseButton.y))
                {
                    int ownerId{ (*it)->getOwnerId() };
                    // if currently iterated object has an owner, and it is not this player, skip to the next one
                    if (ownerId != cnst::defaultOwnerId && playerId != ownerId)
                    {
                        continue;
                    }

                    if (event.mouseButton.button == sf::Mouse::Button::Left)
                    {
                        (*it)->onSelect(event, *this);
                    }

                    // move object to the front if it is clicked on
                    std::rotate(it, it + 1, m_objectList.end());
                    break;
                }
            }
        }

        for (auto it{ m_objectList.rbegin() }; it != m_objectList.rend(); ++it)
        {
            bool handled{ false };

            if (
                (*it)->selected
                && event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Button::Left
            )
            {
                Card *card{ dynamic_cast<Card*>((*it).get()) };

                (*it)->onRelease(event, *this);

                if (card)
                {   
                    // because "it" is a reverse_iterator,
                    // we need to convert it to just iterator (for vector::erase in handleCardDrop)
                    bool cardMovedFromBoard{ handleCardDrop((it).base() - 1) };
                    if (cardMovedFromBoard)
                    {
                        // we don't make the card handle the event,
                        // because iterator pointing to it is removed from the board's list
                        continue;
                    }
                }
            }
            
            // delete a die
            if (
                (*it)->selected
                && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::D
            )
            {
                Die *die{ dynamic_cast<Die*>((*it).get()) };

                if (die)
                {
                    m_objectList.erase((it).base() - 1);
                    continue;
                }
            }

            int ownerId{ (*it)->getOwnerId() };
            // if currently iterated object has an owner, and it is not this player, skip to the next one
            if (ownerId != cnst::defaultOwnerId && playerId != ownerId)
            {
                continue;
            }

            handled = (*it)->handleEvent(event, *this);

            if (handled)
            {
                return true;
            }
        }

        return false;
    }

    void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.clear(m_backgroundColor);

        for (const auto &gameObject : m_objectList)
        {
            target.draw(*gameObject);
        }
    }

    // if Card has been dropped over some container (deck, hand), move it there
    bool Board::handleCardDrop(GameObjectList::iterator cardIt)
    {
        Card *card{ dynamic_cast<Card*>((*cardIt).get()) };

        for (auto it{ m_objectList.rbegin() }; it != m_objectList.rend(); ++it )
        {
            auto ownerId{ (*it)->getOwnerId() };
            if (ownerId != card->getOwnerId() && ownerId != cnst::defaultOwnerId)
            {
                continue;
            }

            Deck *deck{ dynamic_cast<Deck*>((*it).get()) };

            if (deck && deck->getBoundingBox().intersects(card->getBoundingBox()))
            {
                deck->putCardOnTop(*card);
                m_objectList.erase(cardIt);
                return true;
            }

            Hand *hand{ dynamic_cast<Hand*>((*it).get()) };

            if (hand && hand->getBoundingBox().intersects(card->getBoundingBox()))
            {
                hand->putCardIn(*card);
                m_objectList.erase(cardIt);
                return true;
            }
        }

        return false;
    }

    void Board::spawnDie(const sf::Vector2i &mousePos)
    {
        auto die{ std::make_unique<Die>() };
        die->setPosition(mousePos.x, mousePos.y);

        m_objectList.push_back(std::move(die));
    }

    void Board::deepCopy(const Board &board)
    {
        m_window = board.m_window;
        m_backgroundColor = board.m_backgroundColor;

        m_objectList.resize(board.m_objectList.size());
        for (int i{ 0 }; i < m_objectList.size(); ++i)
        {
            m_objectList[i] = board.m_objectList[i]->clone();
        }
    }

    Board& Board::operator=(const Board &board)
    {
        if (&board != this)
        {
            deepCopy(board);
        }

        return *this;
    }

}