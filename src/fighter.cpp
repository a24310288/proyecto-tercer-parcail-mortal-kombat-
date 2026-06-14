#include "fighter.hpp"

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>

Fighter::Fighter(float x, float y, const std::string& imagePath)
{
    speed = 1.5f;

    velocityY = 0.02f;
    gravity = 0.15f;

    isJumping = false;

    groundY = y;

    health = 100;
    attacking = false;
  
    texture = std::make_unique<sf::Texture>();

    if (!texture->loadFromFile(imagePath))
    {
        throw std::runtime_error("No se pudo cargar la textura: " + imagePath);
    }

    sprite = std::make_unique<sf::Sprite>(*texture);

//=============================
// CONFIGURACIÓN DE ANIMACIONES
//=============================

currentAnimation = IDLE;

currentFrame = 0;

frameTime = 0.15f;

//--------------------------------
// IDLE
//--------------------------------

// Estos valores los ajustaremos cuando
// terminemos de medir el spritesheet.

idleFrames.push_back(sf::IntRect(
    sf::Vector2i(0, 0),
    sf::Vector2i(170, 220)
));

idleFrames.push_back(sf::IntRect(
    sf::Vector2i(170, 0),
    sf::Vector2i(170, 220)
));

//--------------------------------
// Al iniciar mostramos el primer frame
//--------------------------------

sprite->setTextureRect(idleFrames[0]);

sprite->setPosition(sf::Vector2f(x, y));

sprite->setScale(sf::Vector2f(0.5f,0.5f));
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
        velocityY = -8.0f;
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

    UpdateAnimation();
}

//====================================================
// PEGA ESTA FUNCIÓN AQUÍ
//====================================================

void Fighter::UpdateAnimation()
{
    if (animationClock.getElapsedTime().asSeconds() < frameTime)
        return;

    animationClock.restart();

    switch (currentAnimation)
    {
        case IDLE:
        {
            currentFrame++;

            if (currentFrame >= idleFrames.size())
                currentFrame = 0;

            sprite->setTextureRect(idleFrames[currentFrame]);
            break;
        }

        case WALK:
            break;

        case ATTACK:
            break;
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
    sf::FloatRect r = sprite->getGlobalBounds();

    // Reducimos bastante la caja de colisión
    r.position.x += 100.f;
    r.size.x -= 160.f;

    // Un poco menos de altura también ayuda
    r.position.y += 10.f;
    r.size.y -= 10.f;

    return r;
}

void Fighter::FaceRight()
{
    sprite->setOrigin(sf::Vector2f(0.f, 0.f));
    sprite->setScale(sf::Vector2f(0.5f, 0.5f));
}

void Fighter::FaceLeft()
{
    sf::IntRect rect = sprite->getTextureRect();

    sprite->setOrigin(
        sf::Vector2f(
            static_cast<float>(rect.size.x),
            0.f
        )
    );

    sprite->setScale(sf::Vector2f(-0.5f, 0.5f));
}


int Fighter::GetHealth() const
{
    return health;
}

void Fighter::TakeDamage(int damage)
{
    health -= damage;

    if (health < 0)
        health = 0;
}

bool Fighter::IsAlive() const
{
    return health > 0;
}

void Fighter::StartAttack()
{
    attacking = true;
}


void Fighter::SetIdle()
{
    currentAnimation = IDLE;
    currentFrame = 0;
}

void Fighter::SetWalk()
{
    currentAnimation = WALK;
    currentFrame = 0;
}

void Fighter::SetAttack()
{
    currentAnimation = ATTACK;
    currentFrame = 0;
}
void Fighter::StopAttack()
{
    attacking = false;
}

bool Fighter::IsAttacking() const
{
    return attacking;
}




