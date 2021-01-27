#pragma once

#include <string>
#include <iostream>

class Card
{
private:
    int m_id;
    std::string m_name;
    std::string m_description;

public:
    Card(int id, const std::string &name, const std::string &description);

    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }
};

std::ostream& operator<<(std::ostream &out, const Card &card);