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

void Fighter::MoveLeft()
{
    fighterShape.move(-speed, 0);
}

void Fighter::MoveRight()
{
    fighterShape.move(speed, 0);
}

void Fighter::Jump()
{
    if (!isJumping)
    {
        velocityY = -12.0f;
        isJumping = true;
    }
}

void Fighter::Update()
{
    velocityY += gravity;

    fighterShape.move(0, velocityY);

    if (fighterShape.getPosition().y >= groundY)
    {
        fighterShape.setPosition(
            fighterShape.getPosition().x,
            groundY
        );

        velocityY = 0;
        isJumping = false;
    }
}

sf::RectangleShape Fighter::GetShape()
{
    return fighterShape;
}

