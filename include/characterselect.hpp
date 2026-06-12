#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class CharacterSelect
{
public:
    CharacterSelect();

    void Run(sf::RenderWindow& window);

    std::string GetPlayer1() const;
    std::string GetPlayer2() const;

private:

    int p1Index;
    int p2Index;

    bool p1Ready;
    bool p2Ready;

    std::string p1Result;
    std::string p2Result;

    sf::Texture textures[4];
    std::vector<sf::Sprite> sprites;

    sf::RectangleShape cursorP1;
    sf::RectangleShape cursorP2;

    sf::Vector2f positions[4];

    void MoveP1(sf::Keyboard::Scancode key);
    void MoveP2(sf::Keyboard::Scancode key);

    void ConfirmP1();
    void ConfirmP2();

    void Draw(sf::RenderWindow& window);
};
