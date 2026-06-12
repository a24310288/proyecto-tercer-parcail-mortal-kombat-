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
    static constexpr int CHARACTER_COUNT = 4;

    std::string selectPaths[4];
    std::string fighterPaths[4];
    sf::Vector2f positions[4];

    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;

    sf::RectangleShape cursorP1;
    sf::RectangleShape cursorP2;

    int p1Index;
    int p2Index;

    bool p1Ready;
    bool p2Ready;

    std::string p1Result;
    std::string p2Result;

    void LoadCharacters();
    void UpdateCursorPositions();
    void MoveP1(sf::Keyboard::Scancode key);
    void MoveP2(sf::Keyboard::Scancode key);
    void ConfirmP1();
    void ConfirmP2();
    void Draw(sf::RenderWindow& window);
};

