#include "fighter.hpp"
#include <iostream>
#include <cmath>

Fighter::Fighter(float x, float y, const std::string& characterName)
{
    health = 100;
    currentAnimation = IDLE;
    currentFrame = 0;

    std::string basePath = "assets/imagenes/sprites/" + characterName;

    // Asignación según las carpetas físicas reales
    if (characterName == "chavo")
    {
        LoadAnimationFolder(basePath, "chavo_basico", 1, idleTextures);
        LoadAnimationFolder(basePath, "chavo_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, "chavo_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, "chavo_ataque", 1, attackTextures);
    }
    else if (characterName == "Kratos" || characterName == "kratos")
    {
        LoadAnimationFolder(basePath, "kratos_basico", 1, idleTextures);
        LoadAnimationFolder(basePath, "kratos_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, "kratos_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, "kratos_ataque", 1, attackTextures);
    }
    else
    {
        LoadAnimationFolder(basePath, characterName + "_basico", 1, idleTextures);
        LoadAnimationFolder(basePath, characterName + "_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, characterName + "_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, characterName + "_ataque", 1, attackTextures);
    }

    // --- CORRECCIÓN SFML 3: Se inicializa el sprite pasándole la textura obligatoriamente ---
    if (!idleTextures.empty() && idleTextures[0])
    {
        sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);
    }
    else
    {
        // Respaldo de emergencia si no cargó ninguna textura
        static sf::Texture dummyTexture;
        sprite = std::make_unique<sf::Sprite>(dummyTexture);
    }

    sprite->setPosition({x, y});
}

void Fighter::LoadAnimationFolder(const std::string& basePath, const std::string& animName, int count, std::vector<std::unique_ptr<sf::Texture>>& container)
{
    std::string fullPath = basePath + "/" + animName + ".png";
    auto texture = std::make_unique<sf::Texture>();
    
    if (texture->loadFromFile(fullPath))
    {
        container.push_back(std::move(texture));
    }
    else
    {
        std::cout << "Alerta: No se encontro la imagen en: " << fullPath << "\n";
        auto fallback = std::make_unique<sf::Texture>();
        container.push_back(std::move(fallback));
    }
}

void Fighter::Update()
{
    // Lógica interna de actualización por frames
}

sf::FloatRect Fighter::GetBounds() const
{
    if (!sprite) return sf::FloatRect();
    return sprite->getGlobalBounds();
}

sf::Vector2f Fighter::GetPosition() const
{
    if (!sprite) return {0.f, 0.f};
    return sprite->getPosition();
}

const sf::Sprite& Fighter::GetSprite() const
{
    return *sprite;
}

int Fighter::GetHealth() const { return health; }

void Fighter::TakeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

bool Fighter::IsAlive() const { return health > 0; }

void Fighter::MoveLeft()
{
    if (sprite) sprite->move({-5.f, 0.f});
    currentAnimation = WALK;
}

void Fighter::MoveRight()
{
    if (sprite) sprite->move({5.f, 0.f});
    currentAnimation = WALK;
}

void Fighter::Jump()
{
    currentAnimation = JUMP;
}

void Fighter::FaceRight()
{
    if (sprite)
    {
        sprite->setScale({std::abs(sprite->getScale().x), sprite->getScale().y});
    }
}

void Fighter::FaceLeft()
{
    if (sprite)
    {
        sprite->setScale({-std::abs(sprite->getScale().x), sprite->getScale().y});
    }
}

void Fighter::StartAttack()
{
    currentAnimation = ATTACK;
    currentFrame = 0;
}

void Fighter::StopAttack() 
{
    // Lógica para detener el ataque si es necesaria
}

bool Fighter::IsAttacking() const 
{ 
    return currentAnimation == ATTACK; 
}

void Fighter::SetIdle() { currentAnimation = IDLE; }
void Fighter::SetWalk() { currentAnimation = WALK; }
void Fighter::SetAttack() { currentAnimation = ATTACK; }

Fighter::~Fighter()
{
    // Destructor explícito perfectamente enlazado
}


