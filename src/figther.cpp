#include "Fighter.hpp"

Fighter::Fighter(float x, float y)
{
   speed = 5.0f;

    velocityY = 0.0f;
    gravity = 0.5f;

    isJumping = false;

    groundY = y;

    texture.loadFromFile("assets/chabo mk.png");

    sprite.setTexture(texture);

    sprite.setPosition(x, y);

    sprite.setScale({0.5f, 0.5f});
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

    sprite.move({0, velocityY});

    if (sprite.getPosition().y >= groundY)
    {
        sprite.setPosition(
            {sprite.getPosition().x, groundY}
        );

        velocityY = 0;
        isJumping = false;
    }
}

sf::Sprite Fighter::GetSprite()
{
    return sprite;
}

