#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>

#include <SFML/Audio.hpp>
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
    sf::Music fightMusic;
    bool musicStarted = false;

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
        if (!musicStarted)
        {
            const std::filesystem::path musicPath = "assets/musica/musica de fondo mk.mp3";

            if (std::filesystem::exists(musicPath))
            {
                if (fightMusic.openFromFile(musicPath.string()))
                {
                    fightMusic.setLooping(true);
                    fightMusic.play();
                    musicStarted = true;
                }
                else
                {
                    std::cerr << "No se pudo abrir la música: " << musicPath << "\n";
                }
            }
            else
            {
                std::cerr << "La ruta de la música no existe: " << musicPath << "\n";
            }
        }

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

    sf::Font font;

    sf::Text textoVida1(font);
    sf::Text textoVida2(font);
    sf::Text textoTiempo(font);
    sf::Text textoFinal(font); 

    textoVida1.setString("P1: " + std::to_string(player1.GetHealth()));
    textoVida2.setString("P2: " + std::to_string(player2.GetHealth()));

    if(!font.openFromFile("assets/fonts/ARIAL.TTF"))
    {
        std::cout << "No se pudo cargar la fuente.\n";
        return -1;
    }

    textoVida1.setCharacterSize(30);
    textoVida2.setCharacterSize(30);
    textoTiempo.setCharacterSize(35);
    textoFinal.setCharacterSize(65); 

    textoVida1.setFillColor(sf::Color::White);
    textoVida2.setFillColor(sf::Color::White);
    textoTiempo.setFillColor(sf::Color::Yellow);
    textoFinal.setFillColor(sf::Color::Red); 

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
                
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                std::cout << "Tecla presionada\n";

                if (keyPressed->scancode == sf::Keyboard::Scancode::F)
                    std::cout << "F detectada\n";

                if (keyPressed->scancode == sf::Keyboard::Scancode::RControl)
                    std::cout << "Right Ctrl detectada\n";
            }
        }

        if(!peleaTerminada)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
            {
                player1.MoveLeft();

                if(player1.GetBounds().findIntersection(player2.GetBounds()))
                    player1.MoveRight();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
            {
                player1.MoveRight();

                if(player1.GetBounds().findIntersection(player2.GetBounds()))
                    player1.MoveLeft();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
                player1.Jump();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
            {
                player2.MoveLeft();

                if(player1.GetBounds().findIntersection(player2.GetBounds()))
                    player2.MoveRight();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
            {
                player2.MoveRight();

                if(player1.GetBounds().findIntersection(player2.GetBounds()))
                    player2.MoveLeft();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                player2.Jump();
        }

        if(!peleaTerminada)
        {
            sf::FloatRect b1 = player1.GetBounds();
            sf::FloatRect b2 = player2.GetBounds();

            float distancia = std::abs(
                (b1.position.x + b1.size.x / 2.f) -
                (b2.position.x + b2.size.x / 2.f)
            );

            bool tocando = distancia < 40.f;

            static bool alreadyHit = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
            {
                if (!alreadyHit && tocando)
                {
                    int damage = 7 + rand() % 2; // 7 u 8

                    player2.TakeDamage(damage);

                    std::cout << "P1 golpea -> -" << damage
                              << " Vida P2: " << player2.GetHealth() << "\n";

                    alreadyHit = true;
                }
            }
            else
            {
                alreadyHit = false;
            }

            static bool alreadyHit2 = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
            {
                if (!alreadyHit2 && tocando)
                {
                    int damage = 7 + rand() % 2; // 7 u 8

                    player1.TakeDamage(damage);

                    std::cout << "P2 golpea -> -" << damage
                              << " Vida P1: " << player1.GetHealth() << "\n";

                    alreadyHit2 = true;
                }
            }
            else
            {
                alreadyHit2 = false;
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

        static int tiempoCongelado = 60;
        int tiempo = tiempoCongelado;

        if(!peleaTerminada)
        {
            tiempo = 60 - reloj.getElapsedTime().asSeconds();

            if(tiempo < 0)
                tiempo = 0;

            tiempoCongelado = tiempo;

            if(tiempo == 0 || player1.GetHealth() <= 0 || player2.GetHealth() <= 0)
            { 
                peleaTerminada = true;
                fightMusic.stop();

                if(player2.GetHealth() <= 0)
                {
                    textoFinal.setString("JUGADOR 1 GANA");
                }
                else if(player1.GetHealth() <= 0)
                {
                    textoFinal.setString("JUGADOR 2 GANA");
                }
                else if(tiempo == 0)
                {
                    textoFinal.setString("TIEMPO AGOTADO");
                }

                sf::FloatRect textBounds = textoFinal.getLocalBounds();
                textoFinal.setPosition({
                    (1280.f - textBounds.size.x) / 2.f,
                    (720.f - textBounds.size.y) / 2.f
                });
            }
        }

        textoVida1.setString("P1: " + std::to_string(player1.GetHealth()));
        textoVida2.setString("P2: " + std::to_string(player2.GetHealth()));
        textoTiempo.setString(std::to_string(tiempo));

        window.clear();

        mapManager.draw(window);

        window.draw(player1.GetSprite());
        window.draw(player2.GetSprite());

        window.draw(textoVida1);
        window.draw(textoVida2);
        window.draw(textoTiempo);

        if(peleaTerminada)
        {
            window.draw(textoFinal);
        }

        window.display();
    }

    return 0;
}



