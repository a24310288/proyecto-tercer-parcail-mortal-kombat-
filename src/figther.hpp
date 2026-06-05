#pragma once

#include <string>
#include <memory>

namespace sf { class Texture; class Sprite; }

class Fighter
{
public:

    Fighter(float x, float y, const std::string& imagePath);
    ~Fighter();

    void MoveLeft();
    void MoveRight();
    void Jump ();
    void Update ();

    const sf::Sprite& GetSprite() const;

private:

    float speed;

    float velocityY;
    float gravity;

    bool isJumping;

    float groundY;

    std::unique_ptr<sf::Texture> texture;
    std::unique_ptr<sf::Sprite> sprite;
};
