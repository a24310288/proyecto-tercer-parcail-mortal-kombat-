#include "fighter.hpp"
#include <iostream>
#include <cmath>

Fighter::Fighter(float x, float y, const std::string& characterName)
{
    health = 100;
    currentAnimation = IDLE;
    currentFrame = 0;

    std::string basePath = "assets/imagenes/sprites/" + characterName;

    // Vinculación EXACTA con las mayúsculas y nombres de tus archivos reales en disco
    if (characterName == "chavo")
    {
        LoadAnimationFolder(basePath, "chavo_basico_1", 1, idleTextures);
        LoadAnimationFolder(basePath, "chavo_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, "chavo_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, "chavo_ataque", 1, attackTextures);
    }
    else if (characterName == "Kratos" || characterName == "kratos")
    {
        // Tu archivo real en disco empieza con K mayúscula: "Kratos_ataque_1.png"
        LoadAnimationFolder(basePath, "Kratos_ataque_1", 1, idleTextures);
        LoadAnimationFolder(basePath, "Kratos_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, "Kratos_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, "Kratos_ataque_1", 1, attackTextures);
    }
    else
    {
        LoadAnimationFolder(basePath, characterName + "_basico_1", 1, idleTextures);
        LoadAnimationFolder(basePath, characterName + "_caminar", 1, walkTextures);
        LoadAnimationFolder(basePath, characterName + "_salto", 1, jumpTextures);
        LoadAnimationFolder(basePath, characterName + "_ataque", 1, attackTextures);
    }

    // Inicializar el Sprite apuntando únicamente al primer cuadro del personaje
    if (!idleTextures.empty() && idleTextures[0])
    {
        sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);
        // Define un rectángulo de recorte inicial razonable para ver el sprite
        sprite->setTextureRect(sf::IntRect({0, 0}, {120, 180}));
    }
    else
    {
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
        // Si falla, creamos una textura de respaldo para evitar que el juego crashee
        auto fallback = std::make_unique<sf::Texture>();
        container.push_back(std::move(fallback));
    }
}

void Fighter::Update()
{
    sf::Texture* currentTex = nullptr;
    
    if (currentAnimation == IDLE && !idleTextures.empty()) currentTex = idleTextures[0].get();
    else if (currentAnimation == WALK && !walkTextures.empty()) currentTex = walkTextures[0].get();
    else if (currentAnimation == JUMP && !jumpTextures.empty()) currentTex = jumpTextures[0].get();
    else if (currentAnimation == ATTACK && !attackTextures.empty()) currentTex = attackTextures[0].get();

    if (currentTex && sprite)
    {
        sprite->setTexture(*currentTex);
        
        int frameWidth = 120; 
        int totalFrames = currentTex->getSize().x / frameWidth;
        if (totalFrames <= 0) totalFrames = 1;

        static float frameCounter = 0;
        frameCounter += 0.15f; 
        if (frameCounter >= 1.f)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            frameCounter = 0;
        }

        sprite->setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, 180}));
    }
}

// CORRECCIÓN DEL SALTO Y CAJA DE DAÑO: Mantiene tanto el ancho como el alto real del sprite
sf::FloatRect Fighter::GetBounds() const
{
    if (!sprite) return sf::FloatRect();
    sf::FloatRect globalBounds = sprite->getGlobalBounds();
    
    // Le asignamos un tamaño de colisión fijo y sólido en ambos ejes para que las físicas de salto y daño funcionen
    globalBounds.size.x = 120.f;
    globalBounds.size.y = 180.f; 
    return globalBounds;
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
    if (sprite) sprite->move({-3.0f, 0.f});
    currentAnimation = WALK;
}

void Fighter::MoveRight()
{
    if (sprite) sprite->move({3.0f, 0.f});
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

void Fighter::StopAttack() {}
bool Fighter::IsAttacking() const { return currentAnimation == ATTACK; }
void Fighter::SetIdle() { currentAnimation = IDLE; }
void Fighter::SetWalk() { currentAnimation = WALK; }
void Fighter::SetAttack() { currentAnimation = ATTACK; }

Fighter::~Fighter() {}
