#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "fighter.hpp"

int main()
{
    std::string selectedCharacter1;
    std::string selectedCharacter2;

    int option1;
    int option2;

    std::cout << "\n===== JUGADOR 1 =====\n";
    std::cout << "1 - chavo \n";
    std::cout << "2 - omniman \n";
    std::cout << "3 - kratos \n";
    std::cout << "4 - rengoku \n";

    std::cin >> option1;

    if(option1 == 1)
        selectedCharacter1 = "assets/chavo.png";

    if(option1 == 2)
        selectedCharacter1 = "assets/omniman.png";

    if(option1 == 3)
        selectedCharacter1 = "assets/kratos.png";

    if(option1 == 4)
        selectedCharacter1 = "assets/rengoku.png";

    std::cout << "\n===== JUGADOR 2 =====\n";
    std::cout << "1 - chavo \n";
    std::cout << "2 - omniman \n";
    std::cout << "3 - kratos \n";
    std::cout << "4 - rengoku \n";

    std::cin >> option2;

    if(option2 == 1)
        selectedCharacter2 = "assets/chavo.png";

    if(option2 == 2)
        selectedCharacter2 = "assets/omniman.png";

    if(option2 == 3)
        selectedCharacter2 = "assets/kratos.png";

    if(option2 == 4)
        selectedCharacter2 = "assets/rengoku.png";

    Fighter player1(
        200,
        500,
        selectedCharacter1
    );

    Fighter player2(
        900,
        500,
        selectedCharacter2
    );

    sf::RenderWindow window(
        sf::VideoMode({1280, 720}),
        "Mortal Kombat"
    );

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player1.MoveLeft();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player1.MoveRight();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            player1.Jump();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            player2.MoveLeft();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            player2.MoveRight();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
        {
            player2.Jump();
        }

        player1.Update();
        player2.Update();

        window.clear();

        window.draw(player1.GetSprite());
        window.draw(player2.GetSprite());

        window.display();
    }

    return 0;
}
