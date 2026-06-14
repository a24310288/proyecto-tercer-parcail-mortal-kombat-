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
           float dx = player1.GetPosition().x - player2.GetPosition().x;
bool tocando = std::abs(dx) < 400.f;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
{
    static bool alreadyHit = false;

    if (!alreadyHit && tocando)
    {
        player2.TakeDamage(10);
        std::cout << "P1 golpea -> vida P2: " << player2.GetHealth() << "\n";
        alreadyHit = true;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
        alreadyHit = false;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
{
    static bool alreadyHit2 = false;

    if (!alreadyHit2 && tocando)
    {
        player1.TakeDamage(10);
        std::cout << "P2 golpea -> vida P1: " << player1.GetHealth() << "\n";
        alreadyHit2 = true;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
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

        if(tiempo == 0 || player1.GetHealth() <= 0 || player2.GetHealth() <= 0)
            peleaTerminada = true;
            fightMusic.stop();
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

        window.display();
    }

    return 0;
}



