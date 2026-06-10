#pragma once

#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class Fighter
{
public:

    Fighter(float x, float y, const std::string& imagePath);
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

private:

    float speed;

    float velocityY;
    float gravity;

    bool isJumping;

    float groundY;

    std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
};
