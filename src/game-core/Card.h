#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

class Card : public GameObject
{
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    sf::Sprite m_faceUpSprite;
    sf::Sprite m_faceDownSprite;

public:
    bool faceUp{ true };

public:
    Card(int id, const std::string &name, const std::string &description);

    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }

    void setFaceUpTexture(const sf::Texture &texture) { m_faceUpSprite.setTexture(texture); }
    void setFaceUpTextureRect(const sf::IntRect& rectangle) { m_faceUpSprite.setTextureRect(rectangle); }
    void setFaceDownTexture(const sf::Texture &texture) { m_faceDownSprite.setTexture(texture); }
    void setFaceDownTextureRect(const sf::IntRect& rectangle) { m_faceDownSprite.setTextureRect(rectangle); }

    void flip() { faceUp = !faceUp; }

    virtual bool contains(int x, int y) const override;
    virtual bool handleEvent(const sf::Event &event) override;
    
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};