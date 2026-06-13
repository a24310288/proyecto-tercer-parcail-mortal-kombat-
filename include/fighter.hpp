#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Fighter
{
public:
    Fighter(float x, float y, const std::string& imagePath);
    ~Fighter();

    // Movimiento
    void MoveLeft();
    void MoveRight();
    void Jump();
    void Update();

    // Sprite
    const sf::Sprite& GetSprite() const;
    void SetPosition(float x, float y);
    sf::Vector2f GetPosition() const;
    sf::FloatRect GetBounds() const;

    // Dirección
    void FaceRight();
    void FaceLeft();

    // ==========================
    // VIDA Y GOLPES (NUEVO)
    // ==========================

    // Devuelve la vida actual (0-100)
    int GetHealth() const;

    // Resta vida
    void TakeDamage(int damage);

    // Saber si sigue vivo
    bool IsAlive() const;

    // Estado de ataque
    void StartAttack();
    void StopAttack();
    bool IsAttacking() const;

private:
    // Movimiento
    float speed;
    float velocityY;
    float gravity;
    bool isJumping;
    float groundY;

    // Vida
    int health;

    // Ataque
    bool attacking;

    // Imagen
    std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
};

