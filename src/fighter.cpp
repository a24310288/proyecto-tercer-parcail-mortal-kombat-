#include "fighter.hpp"

#include <SFML/Graphics.hpp>
#include <stdexcept>

Fighter::Fighter(float x, float y, const std::string& imagePath)
{
    speed = 3.0f;

    velocityY = 0.0f;
    gravity = 0.3f;

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
        velocityY = -18.0f;
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
            sf::Vector2f(
                sprite->getPosition().x,
                groundY
            )
        );

        velocityY = 0;
        isJumping = false;
    }
}

const sf::Sprite& Fighter::GetSprite() const
{
    return *sprite;
}

void Fighter::SetPosition(float x, float y)
{
    sprite->setPosition(sf::Vector2f(x, y));
}

sf::Vector2f Fighter::GetPosition() const
{
    return sprite->getPosition();
}

sf::FloatRect Fighter::GetBounds() const
{
    return sprite->getGlobalBounds();
}

void Fighter::FaceRight()
{
    sprite->setOrigin(sf::Vector2f(0.f, 0.f));
    sprite->setScale(sf::Vector2f(0.5f, 0.5f));
}

void Fighter::FaceLeft()
{
    sprite->setOrigin(
        sf::Vector2f(
            static_cast<float>(texture->getSize().x),
            0.f
        )
    );

    sprite->setScale(sf::Vector2f(-0.5f, 0.5f));
}


