#include "fighter.hpp"
#include <iostream>
#include <cmath>

Fighter::Fighter(float x, float y, const std::string& characterName)
{
    health = 100;
    currentAnimation = IDLE;
    currentFrame = 0;
    animationTimer = 0.f;
    name = characterName;
    
    facingRight = true;
    attacking = false;
    velocityY = 0.f;
    isGrounded = true;

    // RUTA REAL DE TUS CARPETAS (Sin /imagenes/)
    std::string basePath = "assets/sprites/" + characterName;

    if (characterName == "chavo")
    {
        LoadAnimationFolder(basePath, "chavo_basico_1", idleTextures);
        LoadAnimationFolder(basePath, "chavo_caminar", walkTextures);
        LoadAnimationFolder(basePath, "chavo_salto", jumpTextures);
        LoadAnimationFolder(basePath, "chavo_dash", attackTextures); // Archivo real en tu disco
    }
    else // kratos
    {
        LoadAnimationFolder(basePath, "Kratos_ataque_1", idleTextures); // Tu imagen base de Kratos
        LoadAnimationFolder(basePath, "Kratos_caminar", walkTextures);
        LoadAnimationFolder(basePath, "Kratos_salto", jumpTextures);
        LoadAnimationFolder(basePath, "Kratos_dash", attackTextures);
    }

    if (!idleTextures.empty() && idleTextures[0])
    {
        sprite = std::make_unique<sf::Sprite>(*idleTextures[0]);
        sprite->setTextureRect(sf::IntRect({0, 0}, {128, 150}));
    }
    else
    {
        static sf::Texture dummyTexture;
        sprite = std::make_unique<sf::Sprite>(dummyTexture);
    }

    sprite->setPosition({x, y});
}

void Fighter::LoadAnimationFolder(const std::string& basePath, const std::string& animName, std::vector<std::unique_ptr<sf::Texture>>& container)
{
    std::string fullPath = basePath + "/" + animName + ".png";
    auto texture = std::make_unique<sf::Texture>();
    
    if (texture->loadFromFile(fullPath))
    {
        container.push_back(std::move(texture));
    }
    else
    {
        std::cout << "ERROR FATAL: No existe el archivo: " << fullPath << "\n";
        // Textura de emergencia para que el juego NUNCA se divida entre 0
        auto fallback = std::make_unique<sf::Texture>();
        sf::Image img;
        img.resize({128, 150}, sf::Color::Magenta);
        fallback->loadFromImage(img);
        container.push_back(std::move(fallback));
    }
}

void Fighter::Update()
{
    // Gravedad física para que caigan si saltan
    if (sprite)
    {
        sf::Vector2f pos = sprite->getPosition();
        if (pos.y < 450.f) 
        {
            velocityY += 0.6f;
            isGrounded = false;
        }
        else
        {
            pos.y = 450.f;
            velocityY = 0.f;
            isGrounded = true;
            if (currentAnimation == JUMP) currentAnimation = IDLE;
        }
        sprite->setPosition(pos);
        sprite->move({0.f, velocityY});
    }

    sf::Texture* currentTex = nullptr;
    int totalFrames = 1;
    int frameWidth = 128; // Ancho estricto por cuadro para evitar deformaciones

    if (currentAnimation == IDLE && !idleTextures.empty()) {
        currentTex = idleTextures[0].get();
        totalFrames = (name == "chavo") ? 1 : 8;
    }
    else if (currentAnimation == WALK && !walkTextures.empty()) {
        currentTex = walkTextures[0].get();
        totalFrames = (name == "chavo") ? 13 : 8;
    }
    else if (currentAnimation == JUMP && !jumpTextures.empty()) {
        currentTex = jumpTextures[0].get();
        totalFrames = 1;
    }
    else if (currentAnimation == ATTACK && !attackTextures.empty()) {
        currentTex = attackTextures[0].get();
        totalFrames = (name == "chavo") ? 4 : 8;
    }

    if (currentTex && sprite && currentTex->getSize().x > 0)
    {
        sprite->setTexture(*currentTex);

        // Control de velocidad de fotogramas independiente
        animationTimer += (name == "chavo") ? 0.15f : 0.08f; 
        if (animationTimer >= 1.f)
        {
            currentFrame = (currentFrame + 1) % totalFrames;
            animationTimer = 0.f;

            // SI TERMINA LA ANIMACIÓN DE GOLPE, REGRESA A ESTAR QUIETO (IDLE) AUTOMÁTICAMENTE
            if (currentAnimation == ATTACK && currentFrame == 0)
            {
                currentAnimation = IDLE;
                attacking = false;
            }
        }

        int posX = currentFrame * frameWidth;
        if (posX + frameWidth > (int)currentTex->getSize().x) {
            posX = 0;
        }
        
        sprite->setTextureRect(sf::IntRect({posX, 0}, {frameWidth, 150}));
    }

    // Volver a IDLE si se deja de caminar
    if (currentAnimation == WALK && isGrounded)
    {
        static int walkReset = 0;
        if (++walkReset > 3) {
            currentAnimation = IDLE;
            walkReset = 0;
        }
    }
}

sf::FloatRect Fighter::GetBounds() const
{
    if (!sprite) return sf::FloatRect();
    sf::FloatRect bounds = sprite->getGlobalBounds();
    // Forzamos un tamaño físico real para que la colisión del daño en main.cpp SÍ funcione
    bounds.size.x = 110.f;
    bounds.size.y = 150.f;
    return bounds;
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

void Fighter::MoveLeft()
{
    if (sprite) sprite->move({-5.f, 0.f});
    if (currentAnimation != ATTACK && isGrounded) currentAnimation = WALK;
}

void Fighter::MoveRight()
{
    if (sprite) sprite->move({5.f, 0.f});
    if (currentAnimation != ATTACK && isGrounded) currentAnimation = WALK;
}

void Fighter::Jump()
{
    if (isGrounded)
    {
        velocityY = -14.0f;
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
