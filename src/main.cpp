#include <SFML/Graphics.hpp>
#include "Fighter.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "Mortal Kombat"
    );

    Fighter player(200, 500);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player.MoveLeft();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player.MoveRight();
        }

        window.clear();

        window.draw(player.GetShape());

        window.display();
    }

    return 0;
}