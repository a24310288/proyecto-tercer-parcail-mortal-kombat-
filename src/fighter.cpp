#include "fighter.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>

Fighter::Fighter(float x, float y, const std::string& imagePath)
{
    speed = 5.0f;

    velocityY = 0.0f;
    gravity = 0.5f;

    isJumping = false;

    groundY = y;

    texture = std::make_unique<sf::Texture>();

    if (!texture->loadFromFile(imagePath))
    {
        throw std::runtime_error("No se pudo cargar la textura: " + imagePath);
    }

    sprite = std::make_unique<sf::Sprite>(*texture);

    sprite->setPosition(sf::Vector2f(x, y));

    sprite->setScale(sf::Vector2f(0.5f, 0.5f));
}

Fighter::~Fighter() = default;

void Fighter::MoveLeft()
{
    sprite->move(sf::Vector2f(-speed, 0));
}

void Fighter::MoveRight()
{
    sprite->move(sf::Vector2f(speed, 0));
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

    sprite->move(sf::Vector2f(0, velocityY));

    if (sprite->getPosition().y >= groundY)
    {
        sprite->setPosition(
            sf::Vector2f(sprite->getPosition().x, groundY)
        );

        velocityY = 0;
        isJumping = false;
    }
}

const sf::Sprite& Fighter::GetSprite() const
{
    return *sprite;
}


