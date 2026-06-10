#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "fighter.hpp"

int main()
{
    
    sf::RenderWindow startWindow(
        sf::VideoMode({1280u, 720u}),
        "Mortal Kombat"
    );

    sf::Texture portadaTexture;

    if(!portadaTexture.loadFromFile("assets/imagenes/portada.png"))
    {
        return -1;
    }

    sf::Sprite portadaSprite(portadaTexture);

    bool startScreen = true;

    while(startWindow.isOpen() && startScreen)
    {
        while(auto event = startWindow.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                startWindow.close();
                return 0;
            }

            if(event->is<sf::Event::KeyPressed>())
            {
                startScreen = false;
            }
        }

        startWindow.clear();

        startWindow.draw(portadaSprite);

        startWindow.display();
    }

    std::string selectedCharacter1;
    std::string selectedCharacter2;

    int option1;
    int option2;

    std::cout << "\n===== JUGADOR 1 =====\n";
    std::cout << "1 - chavo\n";
    std::cout << "2 - omniman\n";
    std::cout << "3 - kratos\n";
    std::cout << "4 - rengoku\n";

    std::cin >> option1;

    if(option1 == 1)
        selectedCharacter1 = "assets/imagenes/chavo.jpeg";

    if(option1 == 2)
        selectedCharacter1 = "assets/imagenes/omniman.jpeg";

    if(option1 == 3)
        selectedCharacter1 = "assets/imagenes/kratos.jpeg";

    if(option1 == 4)
        selectedCharacter1 = "assets/imagenes/rengoku.jpeg";

    std::cout << "\n===== JUGADOR 2 =====\n";
    std::cout << "1 - chavo\n";
    std::cout << "2 - omniman\n";
    std::cout << "3 - kratos\n";
    std::cout << "4 - rengoku\n";

    std::cin >> option2;

    if(option2 == 1)
        selectedCharacter2 = "assets/imagenes/chavo.jpeg";

    if(option2 == 2)
        selectedCharacter2 = "assets/imagenes/omniman.jpeg";

    if(option2 == 3)
        selectedCharacter2 = "assets/imagenes/kratos.jpeg";

    if(option2 == 4)
        selectedCharacter2 = "assets/imagenes/rengoku.jpeg";

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
        sf::VideoMode({1280u, 720u}),
        "Mortal Kombat"
    );

    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
        {
            player1.MoveLeft();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
        {
            player1.MoveRight();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
        {
            player1.Jump();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
        {
            player2.MoveLeft();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
        {
            player2.MoveRight();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
        {
            player2.Jump();
        }

        player1.Update();
        player2.Update();

        if(player1.GetPosition().x < player2.GetPosition().x)
        {
            player1.FaceRight();
            player2.FaceLeft();
        }
        else
        {
            player1.FaceLeft();
            player2.FaceRight();
        }

        auto p1Bounds = player1.GetBounds();
        auto p2Bounds = player2.GetBounds();

        if(auto overlap = p1Bounds.findIntersection(p2Bounds))
        {
            float correction = overlap->size.x / 2.f;

            if(player1.GetPosition().x < player2.GetPosition().x)
            {
                player1.SetPosition(
                    player1.GetPosition().x - correction,
                    player1.GetPosition().y
                );

                player2.SetPosition(
                    player2.GetPosition().x + correction,
                    player2.GetPosition().y
                );
            }
            else
            {
                player1.SetPosition(
                    player1.GetPosition().x + correction,
                    player1.GetPosition().y
                );

                player2.SetPosition(
                    player2.GetPosition().x - correction,
                    player2.GetPosition().y
                );
            }
        }

        window.clear();

        window.draw(player1.GetSprite());
        window.draw(player2.GetSprite());

        window.display();
    }

    return 0;
}
