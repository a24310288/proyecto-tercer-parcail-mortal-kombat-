#include "fighter.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

// Función de carga con rutas en formato guion bajo para SFML 3
void Fighter::LoadAnimationFolder(const std::string& folderPath, const std::string& prefix, int frameCount, std::vector<std::unique_ptr<sf::Texture>>& targetVector)
{
    for (size_t i = 1; i <= static_cast<size_t>(frameCount); ++i)
    {
        auto tex = std::make_unique<sf::Texture>();
        
        // Formato con guion bajo: ej. "chavo_basico_1.png"
        std::string filePath = folderPath + "/" + prefix + "_" + std::to_string(i) + ".png";
        
        if (tex->loadFromFile(filePath))
        {
            targetVector.push_back(std::move(tex));
        }
        else
        {
            std::cerr << "Alerta: No se encontro la imagen en: " << filePath << "\n";
        }
    }
}

Fighter::Fighter(float x, float y, const std::string& characterName)
{
    speed = 4.0f;
    velocityY = 0.0f;
    gravity = 0.5f;
    isJumping = false;
    groundY = y;
    health = 100;
    attacking = false;
    isMovingThisFrame = false;

    currentAnimation = IDLE;
    currentFrame = 0;
    frameTime = 0.12f; 

    std::string basePath = "assets/imagenes/sprites/" + characterName;

    // Asignación de carpetas y archivos base
    if (characterName == "chavo")
    {
        LoadAnimationFolder(basePath, "chavo_basico", 4, idleTextures);
        LoadAnimationFolder(basePath, "chavo_caminar", 4, walkTextures);
        LoadAnimationFolder(basePath, "chavo_salto", 2, jumpTextures);
        LoadAnimationFolder(basePath, "chavo_ataque", 4, attackTextures); 
    }
    else if (characterName == "kratos")
    {
        LoadAnimationFolder(basePath, "kratos_ataque", 4, idleTextures);  
        LoadAnimationFolder(basePath, "kratos_caminar", 4, walkTextures);
        LoadAnimationFolder(basePath, "kratos_salto", 2, jumpTextures);
        LoadAnimationFolder(basePath, "kratos_ataque", 4, attackTextures);
    }
    else 
    {
        LoadAnimationFolder(basePath, characterName + "_basico", 4, idleTextures);
        LoadAnimationFolder(basePath, characterName + "_caminar", 4, walkTextures);
        LoadAnimationFolder(basePath, characterName + "_salto", 2, jumpTextures);
        LoadAnimationFolder(basePath, characterName + "_ataque", 4, attackTextures);
    }

    sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);

    if (idleTextures.empty())
    {
        // Textura dummy limpia
        auto dummyTex = std::make_unique<sf::Texture>();
        idleTextures.push_back(std::move(dummyTex));
        sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);
    }

    sprite->setPosition(sf::Vector2f(x, groundY));
    sprite->setScale(sf::Vector2f(1.5f, 1.5f));
    sprite->setColor(sf::Color::White); 
}

Fighter::~Fighter() = default;

void Fighter::MoveLeft()
{
    sprite->move(sf::Vector2f(-speed, 0));
    isMovingThisFrame = true;
}

void Fighter::MoveRight()
{
    sprite->move(sf::Vector2f(speed, 0));
    isMovingThisFrame = true;
}

void Fighter::Jump()
{
    if (!isJumping)
    {
        velocityY = -12.0f;
        isJumping = true;
        currentAnimation = JUMP;
        currentFrame = 0;
    }
}

void Fighter::Update()
{
    velocityY += gravity;
    sprite->move(sf::Vector2f(0, velocityY));

    if (sprite->getPosition().y >= groundY)
    {
        sprite->setPosition(sf::Vector2f(sprite->getPosition().x, groundY));
        velocityY = 0;
        isJumping = false;
    }

    if (attacking)
    {
        currentAnimation = ATTACK;
    }
    else if (isJumping)
    {
        currentAnimation = JUMP;
    }
    else if (isMovingThisFrame)
    {
        if (currentAnimation != WALK)
        {
            currentAnimation = WALK;
            currentFrame = 0;
        }
    }
    else
    {
        if (currentAnimation != IDLE)
        {
            currentAnimation = IDLE;
            currentFrame = 0;
        }
    }

    UpdateAnimation();
    isMovingThisFrame = false; 
}

void Fighter::UpdateAnimation()
{
    if (animationClock.getElapsedTime().asSeconds() < frameTime)
        return;

    animationClock.restart();

    switch (currentAnimation)
    {
        case IDLE:
            if (idleTextures.empty()) return;
            currentFrame = (currentFrame + 1) % idleTextures.size();
            sprite->setTexture(*idleTextures[currentFrame], true);
            break;

        case WALK:
            if (walkTextures.empty()) return;
            currentFrame = (currentFrame + 1) % walkTextures.size();
            sprite->setTexture(*walkTextures[currentFrame], true);
            break;

        case JUMP:
            if (jumpTextures.empty()) return;
            if (currentFrame < jumpTextures.size() - 1)
                currentFrame++;
            sprite->setTexture(*jumpTextures[currentFrame], true);
            break;

        case ATTACK:
            if (attackTextures.empty()) return;
            currentFrame++;
            if (currentFrame >= attackTextures.size())
            {
                currentFrame = 0;
                attacking = false;
                currentAnimation = IDLE;
                if (!idleTextures.empty()) sprite->setTexture(*idleTextures[0], true);
            }
            else
            {
                sprite->setTexture(*attackTextures[currentFrame], true);
            }
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
    sf::FloatRect originalBounds = sprite->getGlobalBounds();
    sf::FloatRect adjustedBounds;
    adjustedBounds.position.x = originalBounds.position.x + 15.f;
    adjustedBounds.position.y = originalBounds.position.y;
    adjustedBounds.size.x = originalBounds.size.x - 30.f;
    adjustedBounds.size.y = originalBounds.size.y;
    return adjustedBounds;
}

void Fighter::FaceRight()
{
    sprite->setOrigin(sf::Vector2f(0.f, 0.f));
    float absScaleX = std::abs(sprite->getScale().x);
    float absScaleY = std::abs(sprite->getScale().y);
    sprite->setScale(sf::Vector2f(absScaleX, absScaleY));
}

void Fighter::FaceLeft()
{
    float absScaleX = std::abs(sprite->getScale().x);
    float absScaleY = std::abs(sprite->getScale().y);
    
    auto textureSize = sprite->getTexture().getSize();
    sprite->setOrigin(sf::Vector2f(static_cast<float>(textureSize.x), 0.f));
    sprite->setScale(sf::Vector2f(-absScaleX, absScaleY));
}

int Fighter::GetHealth() const
{
    return health;
}

void Fighter::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

bool Fighter::IsAlive() const
{
    return health > 0;
}

void Fighter::StartAttack()
{
    if (!attacking)
    {
        attacking = true;
        currentAnimation = ATTACK;
        currentFrame = 0;
    }
}

void Fighter::StopAttack()
{
    attacking = false;
}

bool Fighter::IsAttacking() const
{
    return attacking;
}

void Fighter::SetIdle() { currentAnimation = IDLE; }
void Fighter::SetWalk() { currentAnimation = WALK; }
void Fighter::SetAttack() { currentAnimation = ATTACK; }
