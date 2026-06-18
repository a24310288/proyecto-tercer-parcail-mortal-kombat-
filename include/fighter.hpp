#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>

class Fighter {
public:
void Move(float dx);
void SetPosition(sf::Vector2f pos);
    enum AnimationState { IDLE, WALK, JUMP, ATTACK };

    Fighter(float x, float y, const std::string& characterName);
    ~Fighter();

    void Update();
    void MoveLeft();
    void MoveRight();
    void Jump();
    void StartAttack();
    void StopAttack();
    
    void TakeDamage(int damage);
    void SetIdle();
    void SetWalk();
    void SetAttack();
    void FaceLeft();
    void FaceRight();

    int GetHealth() const;
    bool IsAlive() const;
    bool IsAttacking() const;
    sf::FloatRect GetBounds() const;
    sf::Vector2f GetPosition() const;
    const sf::Sprite& GetSprite() const;

private:
    void LoadAnimationFolder(const std::string& basePath, const std::string& animName, std::vector<std::unique_ptr<sf::Texture>>& container);

    int health;
    AnimationState currentAnimation;
    int currentFrame;
    float animationTimer; 
    std::string name;

    bool facingRight;
    bool attacking;
    float velocityY;
    bool isGrounded;

    std::unique_ptr<sf::Sprite> sprite;
    std::vector<std::unique_ptr<sf::Texture>> idleTextures;
    std::vector<std::unique_ptr<sf::Texture>> walkTextures;
    std::vector<std::unique_ptr<sf::Texture>> jumpTextures;
    std::vector<std::unique_ptr<sf::Texture>> attackTextures;
};

#endif
