#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "fighter.hpp"
#include "characterselect.hpp"
#include "MapManager.hpp"

int main()
{
    std::srand(std::time(nullptr));

    sf::RenderWindow window(
        sf::VideoMode({1280u, 720u}),
        "Mortal Kombat"
    );

    sf::Texture portadaTexture;

    if(!portadaTexture.loadFromFile("assets/imagenes/portada.png"))
        return -1;

    sf::Sprite portadaSprite(portadaTexture);

    portadaSprite.setScale(
    {
        1280.f / portadaTexture.getSize().x,
        720.f / portadaTexture.getSize().y
    });

    bool startScreen = true;

    while(window.isOpen() && startScreen)
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();

            if(event->is<sf::Event::KeyPressed>())
                startScreen = false;
        }

        window.clear();
        window.draw(portadaSprite);
        window.display();
    }

    CharacterSelect select;
    select.Run(window);

    MapManager mapManager;
    mapManager.loadRandomMap();

    Fighter player1(
        200,
        500,
        select.GetPlayer1()
    );

    Fighter player2(
        900,
        500,
        select.GetPlayer2()
    );

    int vida1 = 100;
    int vida2 = 100;

    sf::Font font;

    if(!font.openFromFile("assets/fonts/ARIAL.TTF"))
    {
        std::cout << "No se pudo cargar la fuente.\n";
        return -1;
    }

    sf::Text textoVida1(font);
    sf::Text textoVida2(font);
    sf::Text textoTiempo(font);

    textoVida1.setCharacterSize(30);
    textoVida2.setCharacterSize(30);
    textoTiempo.setCharacterSize(35);

    textoVida1.setFillColor(sf::Color::White);
    textoVida2.setFillColor(sf::Color::White);
    textoTiempo.setFillColor(sf::Color::Yellow);

    textoVida1.setPosition({20.f,20.f});
    textoVida2.setPosition({1000.f,20.f});
    textoTiempo.setPosition({610.f,20.f});

    sf::Clock reloj;
    bool peleaTerminada = false;

    bool golpeP1 = false;
    bool golpeP2 = false;

    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        if(!peleaTerminada)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
                player1.MoveLeft();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
                player1.MoveRight();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
                player1.Jump();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
                player2.MoveLeft();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
                player2.MoveRight();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                player2.Jump();
        }

        if(!peleaTerminada)
        {
            auto p1Bounds = player1.GetBounds();
            auto p2Bounds = player2.GetBounds();

            auto centro1 = player1.GetBounds();
auto centro2 = player2.GetBounds();

float distancia =
    std::abs(
        (centro1.position.x + centro1.size.x / 2.f) -
        (centro2.position.x + centro2.size.x / 2.f)
    );

bool tocando = distancia <= 180.f;


            // F
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::G))
{
    if(!golpeP1 && tocando)
    {
        vida2 -= 7 + rand() % 2;

        if(vida2 < 0)
            vida2 = 0;
    }

    golpeP1 = true;
}
else
{
    golpeP1 = false;
}

            // RIGHT CTRL
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
{
    if(!golpeP2 && tocando)
    {
        vida1 -= 7 + rand() % 2;

        if(vida1 < 0)
            vida1 = 0;
    }

    golpeP2 = true;
}
else
{
    golpeP2 = false;
}
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
                player1.SetPosition(player1.GetPosition().x - correction,
                                    player1.GetPosition().y);

                player2.SetPosition(player2.GetPosition().x + correction,
                                    player2.GetPosition().y);
            }
            else
            {
                player1.SetPosition(player1.GetPosition().x + correction,
                                    player1.GetPosition().y);

                player2.SetPosition(player2.GetPosition().x - correction,
                                    player2.GetPosition().y);
            }
        }

        int tiempo = 60 - reloj.getElapsedTime().asSeconds();

        if(tiempo < 0)
            tiempo = 0;

        if(tiempo == 0 || vida1 <= 0 || vida2 <= 0)
            peleaTerminada = true;

        textoVida1.setString("P1: " + std::to_string(vida1));
        textoVida2.setString("P2: " + std::to_string(vida2));
        textoTiempo.setString(std::to_string(tiempo));

        window.clear();

        mapManager.draw(window);

        window.draw(player1.GetSprite());
        window.draw(player2.GetSprite());

        window.draw(textoVida1);
        window.draw(textoVida2);
        window.draw(textoTiempo);

        window.display();
    }

    return 0;
}



