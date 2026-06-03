#include "Fighter.hpp"

Fighter::Fighter(float x, float y)
{
    speed = 5.0f;

    fighterShape.setSize(sf::Vector2f(50.f, 100.f));
    fighterShape.setPosition(x, y);
    fighterShape.setFillColor(sf::Color::Blue);
}

void Fighter::MoveLeft()
{
    fighterShape.move(-speed, 0);
}

void Fighter::MoveRight()
{
    fighterShape.move(speed, 0);
}

sf::RectangleShape Fighter::GetShape()
{
    return fighterShape;
}