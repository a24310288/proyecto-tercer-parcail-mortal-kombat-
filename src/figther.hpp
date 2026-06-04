#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Fighter
{
public:

    Fighter(float x, float y, const std::string& imagePath);

    void MoveLeft();
    void MoveRight();
    void Jump ();

    sf::Sprite GetSprite();

private:

    float speed;

    float velocityY;
    float gravity;

    bool isJumping;

    float groundY;

    sf::Texture texture;
    sf::Sprite sprite;
};
