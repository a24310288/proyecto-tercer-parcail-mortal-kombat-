#include "fighter.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>

Fighter::Fighter(float x, float y, const std::string& characterName)
{
    health = 100;
    currentAnimation = IDLE;
    currentFrame = 0;
    animationTimer = 0.f;
    
    // Convertir el nombre a minúsculas para evitar errores de mayúsculas/minúsculas en main.cpp
    std::string lowerName = characterName;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    name = lowerName;
    
    facingRight = true;
    attacking = false;
    velocityY = 0.f;
    isGrounded = true;

    std::string basePath = "assets/sprites/" + name;

    if (name == "chavo")
    {
        LoadAnimationFolder(basePath, "chavo_basico_1", idleTextures);
        LoadAnimationFolder(basePath, "chavo_caminar", walkTextures);
        LoadAnimationFolder(basePath, "chavo_salto", jumpTextures);
        LoadAnimationFolder(basePath, "chavo_dash", attackTextures); 
    }
    else 
    {
        name = "kratos"; 
        LoadAnimationFolder(basePath, "kratos_ataque_1", idleTextures);
        LoadAnimationFolder(basePath, "kratos_caminar", walkTextures);
        LoadAnimationFolder(basePath, "kratos_salto", jumpTextures);
        LoadAnimationFolder(basePath, "kratos_dash", attackTextures);
    }

    if (idleTextures.empty() || !idleTextures[0] || idleTextures[0]->getSize().x == 0)
    {
        std::cout << "Advertencia: Usando textura de respaldo para evitar parpadeos descontrolados.\n";
        auto fallbackTex = std::make_unique<sf::Texture>();
        sf::Image img;
        img.resize({128, 150}, sf::Color::Red);
        fallbackTex->loadFromImage(img);
        idleTextures.clear();
        idleTextures.push_back(std::move(fallbackTex));
    }

    sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);
    sprite->setTextureRect(sf::IntRect({0, 0}, {128, 150}));
    sprite->setPosition({x, y});
    sprite->setOrigin({64.f, 75.f}); 
}
void Fighter::LoadAnimationFolder(
    const std::string& basePath,
    const std::string& animName,
    std::vector<std::unique_ptr<sf::Texture>>& container)
{
    auto tex = std::make_unique<sf::Texture>();

    std::string ruta = basePath + "/" + animName + ".png";

    if (!tex->loadFromFile(ruta))
    {
        std::cout << "No se pudo cargar: " << ruta << std::endl;
        return;
    }

    container.push_back(std::move(tex));
}

void Fighter::Update()
{
if (sprite)
{
    if (currentAnimation != ATTACK)
    {
        if (!isGrounded)
        {
            velocityY += 0.6f;
            sprite->move({0.f, velocityY});

            if (sprite->getPosition().y >= 500.f)
            {
                sprite->setPosition({sprite->getPosition().x, 500.f});
                velocityY = 0.f;
                isGrounded = true;

                if (currentAnimation == JUMP)
                    currentAnimation = IDLE;
            }
        }
    }
}

    sf::Texture* currentTex = nullptr;
    int totalFrames = 1;

    if (currentAnimation == IDLE && !idleTextures.empty())
    {
        currentTex = idleTextures[0].get();
        totalFrames = (name == "chavo") ? 1 : 8;
    }
    else if (currentAnimation == WALK && !walkTextures.empty())
    {
        currentTex = walkTextures[0].get();
        totalFrames = (name == "chavo") ? 13 : 8;
    }
    else if (currentAnimation == JUMP && !jumpTextures.empty())
    {
        currentTex = jumpTextures[0].get();
        totalFrames = 10;
    }
    else if (currentAnimation == ATTACK && !attackTextures.empty())
    {
        currentTex = attackTextures[0].get();
        totalFrames = (name == "chavo") ? 4 : 8;
    }

    if (!currentTex)
        return;

    sprite->setTexture(*currentTex);

    int frameWidth = currentTex->getSize().x / totalFrames;
    int frameHeight = currentTex->getSize().y;

    animationTimer += 0.016f;
if (animationTimer >= 0.1f)
    {
        animationTimer = 0.f;

        currentFrame++;

        if (currentFrame >= totalFrames)
        {
            currentFrame = 0;

            if (currentAnimation == ATTACK)
            {
                currentAnimation = IDLE;
                attacking = false;
            }
        }
    }

    sprite->setTextureRect(
        sf::IntRect(
            {currentFrame * frameWidth, 0},
            {frameWidth, frameHeight}
        )
    );
   

}


sf::FloatRect Fighter::GetBounds() const
{
    if (!sprite)
        return sf::FloatRect();

    return sprite->getGlobalBounds();
}

sf::Vector2f Fighter::GetPosition() const
{
    if (!sprite) return {0.f, 0.f};
    return sprite->getPosition();
}

const sf::Sprite& Fighter::GetSprite() const { return *sprite; }
int Fighter::GetHealth() const { return health; }

void Fighter::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

bool Fighter::IsAlive() const { return health > 0; }

void Fighter::Jump()
{
    if (isGrounded)
    {
        velocityY = -13.5f;
        currentAnimation = JUMP;
        isGrounded = false;
    }
}

void Fighter::FaceRight()
{
    facingRight = true;
    if (sprite && sprite->getScale().x < 0) sprite->setScale({std::abs(sprite->getScale().x), sprite->getScale().y});
}

void Fighter::FaceLeft()
{
    facingRight = false;
    if (sprite && sprite->getScale().x > 0) sprite->setScale({-std::abs(sprite->getScale().x), sprite->getScale().y});
}

void Fighter::StartAttack()
{
    
    if (currentAnimation != ATTACK)
    {
        currentAnimation = ATTACK;
        currentFrame = 0;
        animationTimer = 0.f;
        attacking = true;
    }
}

void Fighter::StopAttack() {
    attacking = false;
    if (currentAnimation == ATTACK) currentAnimation = IDLE;
}

bool Fighter::IsAttacking() const { return currentAnimation == ATTACK; }
void Fighter::SetIdle() { currentAnimation = IDLE; }
void Fighter::SetWalk() { currentAnimation = WALK; }
void Fighter::SetAttack() { currentAnimation = ATTACK; }

Fighter::~Fighter() {}
void Fighter::Move(float dx)
{
    if (sprite)
        sprite->move({dx, 0.f});
}

void Fighter::SetPosition(sf::Vector2f pos)
{
    if (sprite)
        sprite->setPosition(pos);
}
