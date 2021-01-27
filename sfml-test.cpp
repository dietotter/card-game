#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>

// Idea: have a class for all the program setup (font, textures, etc)

void drawSampleTexture(sf::RenderWindow &window)
{
    sf::Texture texture;

    if (!texture.loadFromFile("images/dino.png", sf::IntRect(180, 87, 303, 513)))
    {
        throw std::runtime_error("Dino texture couldn't be loaded");
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(50.f, 50.f));

    sf::CircleShape texturedCircle(50);
    texturedCircle.setTexture(&texture);

    window.draw(sprite);
    window.draw(texturedCircle);
}

void drawSampleFont(sf::RenderWindow &window)
{
    sf::Font font;
    if (!font.loadFromFile("fonts/cour.ttf"))
    {
        throw std::runtime_error("New Courier font couldn't be loaded");
    }

    sf::Text text;

    text.setFont(font);
    text.setString("Kek");
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::Green);
    text.setStyle(sf::Text::Bold | sf::Text::Italic | sf::Text::Underlined);

    window.draw(text);
}

void drawSampleShapes(sf::RenderWindow &window)
{
    sf::CircleShape circle(50.f);

    circle.setFillColor(sf::Color(255, 0, 255));
    circle.setOutlineThickness(10.f);
    circle.setOutlineColor(sf::Color(250, 150, 100, 127));
    circle.setPosition(sf::Vector2f(400.f, 250.f));

    window.draw(circle);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

        }

        window.clear(sf::Color::Black);

        drawSampleTexture(window);
        drawSampleFont(window);
        drawSampleShapes(window);

        window.display();
    }

    return 0;
}