#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

class Fighter
{
public:
    Fighter(float x, float y, const std::string& characterName);
    ~Fighter();

    void MoveLeft();
    void MoveRight();
    void Jump();
    void Update();

    const sf::Sprite& GetSprite() const;
    void SetPosition(float x, float y);
    sf::Vector2f GetPosition() const;
    sf::FloatRect GetBounds() const;

    void FaceRight();
    void FaceLeft();

    int GetHealth() const;
    void TakeDamage(int damage);
    bool IsAlive() const;

    void StartAttack();
    void StopAttack();
    bool IsAttacking() const;

    void SetIdle();
    void SetWalk();
    void SetAttack();

private:
    // Movimiento y Física
    float speed;
    float velocityY;
    float gravity;
    bool isJumping;
    float groundY;

    int health;
    bool attacking;

    // Puntero al sprite principal
    std::unique_ptr<sf::Sprite> sprite;

    enum AnimationState
    {
        IDLE,
        WALK,
        JUMP,
        ATTACK
    };

    AnimationState currentAnimation;
    
    // Contenedores de texturas dinámicas
    std::vector<sf::Texture> idleTextures;
    std::vector<sf::Texture> walkTextures;
    std::vector<sf::Texture> jumpTextures;
    std::vector<sf::Texture> attackTextures;

    size_t currentFrame;
    sf::Clock animationClock;
    float frameTime;
    
    bool isMovingThisFrame;

    void UpdateAnimation();
    void LoadAnimationFolder(const std::string& folderPath, const std::string& prefix, int frameCount, std::vector<sf::Texture>& targetVector);
};

