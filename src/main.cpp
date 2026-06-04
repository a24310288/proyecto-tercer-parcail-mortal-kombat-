#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Fighter.hpp"

int main()
{
    std::string selectedCharacter;

    int option;

    std::cout << "===== SELECCIONA PERSONAJE =====\n";
    std::cout << "1 - Chabo mk\n";
    std::cout << "2 - Omniman mk\n";
    std::cout << "3 - kratos\n";
    std::cout << "4 - rengoku\n";

    std::cin >> option;

    if(option == 1)
    {
        selectedCharacter = "assets/chavo.png";
    }

    if(option == 2)
    {
        selectedCharacter = "assets/omniman.png";
    }

    if(option == 3)
    {
        selectedCharacter = "assets/kratos.png";
    }

    if(option == 4)
    {
        selectedCharacter = "assets/rengoku.png";
    }
    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "Mortal Kombat"
    );

    Fighter player(
        200,
        500,
        selectedCharacter
    );


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

         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            player.Jump();
        }
         
        player.Update();

        window.clear();

        window.draw(player.GetSprite());

        window.display();
    }

    return 0;
}

