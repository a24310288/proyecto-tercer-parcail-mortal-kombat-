#pragma once

#include <SFML/Graphics.hpp>

class Fighter
{
public:

    Fighter(float x, float y);

    void MoveLeft();
    void MoveRight();

    sf::RectangleShape GetShape();

private:

    float speed;
    sf::RectangleShape fighterShape;
};