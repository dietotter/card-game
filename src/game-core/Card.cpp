#include "Card.h"

Card::Card(int id, const std::string &name, const std::string &description)
    : m_id{ id }, m_name{ name }, m_description{ description }
{
}

std::ostream& operator<<(std::ostream &out, const Card &card)
{
    out << "Card #" << card.getId() << ": " << card.getName() << '\n' << card.getDescription();
    return out;
}