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

std::string obtenerNombreLimpio(const std::string& rutaOTexto)
{
    if (rutaOTexto.find('/') != std::string::npos || rutaOTexto.find('\\') != std::string::npos)
    {
        std::filesystem::path p(rutaOTexto);
        return p.stem().string(); 
    }
    return rutaOTexto;
}

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

    sf::Font font;
    if(!font.openFromFile("assets/fonts/ARIAL.TTF"))
    {
        std::cout << "No se pudo cargar la fuente.\n";
        return -1;
    }

    sf::Text textoVida1(font);
    sf::Text textoVida2(font);
    sf::Text textoTiempo(font);
    sf::Text textoFinal(font); 

    textoVida1.setCharacterSize(30);
    textoVida2.setCharacterSize(30);
    textoTiempo.setCharacterSize(35);
    textoFinal.setCharacterSize(50); 

    textoVida1.setFillColor(sf::Color::White);
    textoVida2.setFillColor(sf::Color::White);
    textoTiempo.setFillColor(sf::Color::Yellow);
    textoFinal.setFillColor(sf::Color::Red); 

    textoVida1.setPosition({20.f,20.f});
    textoVida2.setPosition({950.f,20.f}); 
    textoTiempo.setPosition({550.f,20.f});

    while(window.isOpen())
    {
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
                {
                    window.close();
                    return 0;
                }

                if(event->is<sf::Event::KeyPressed>())
                    startScreen = false;
            }

            window.clear();
            window.draw(portadaSprite);
            window.display();
        }

        if (!window.isOpen()) break;

        CharacterSelect select;
        select.Run(window);

        MapManager mapManager;
        int rondaActual = 1;
        int rondasGanadasP1 = 0;
        int rondasGanadasP2 = 0;

        while(rondaActual <= 3 && window.isOpen())
        {
            mapManager.loadRandomMap();

            std::string p1Nombre = obtenerNombreLimpio(select.GetPlayer1());
            std::string p2Nombre = obtenerNombreLimpio(select.GetPlayer2());

            Fighter player1(200, 500, p1Nombre);
            Fighter player2(900, 500, p2Nombre);

            sf::Clock reloj;
            bool peleaTerminada = false;
            int tiempoCongelado = 60;
            while(window.isOpen() && !peleaTerminada)
            {
                while(auto event = window.pollEvent())
                {
                    if(event->is<sf::Event::Closed>())
                        window.close();
                        
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        std::cout << "Tecla presionada en Ronda " << rondaActual << "\n";

                        if (keyPressed->scancode == sf::Keyboard::Scancode::F)
                            std::cout << "F detectada\n";

                        if (keyPressed->scancode == sf::Keyboard::Scancode::RControl)
                            std::cout << "Right Ctrl detectada\n";
                    }
                }

if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) &&
    !sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) &&
    !player1.IsAttacking())
{
    player1.SetIdle();
}

if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) &&
    !sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) &&
    !player2.IsAttacking())
{
    player2.SetIdle();
}


if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A))
{
    player1.Move(-5.f);

    if (player1.GetBounds().findIntersection(player2.GetBounds()).has_value())
        player1.Move(5.f); 
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
{
    player1.Move(5.f);

    if (player1.GetBounds().findIntersection(player2.GetBounds()).has_value())
        player1.Move(-5.f); 
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W))
{
    player1.Jump();
}


if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
{
    player2.Move(-5.f);

    if (player1.GetBounds().findIntersection(player2.GetBounds()).has_value())
        player2.Move(5.f); 
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
{
    player2.Move(5.f);

    if (player1.GetBounds().findIntersection(player2.GetBounds()).has_value())
        player2.Move(-5.f); 
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
{
    player2.Jump();
}

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
                    player2.Jump();

sf::FloatRect b1 = player1.GetBounds();
sf::FloatRect b2 = player2.GetBounds();

bool tocando = b1.findIntersection(b2).has_value();

static bool alreadyHit = false;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::F))
{
    player1.StartAttack();

    if (!alreadyHit && tocando)
    {
        int damage = 7 + rand() % 2;
        player2.TakeDamage(damage);

        std::cout
            << "Jugador 1 golpea. Vida P2 = "
            << player2.GetHealth()
            << std::endl;

        alreadyHit = true;
    }
}
else
{
    alreadyHit = false;
    player1.StopAttack();
}

static bool alreadyHit2 = false;

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::RControl))
{
    player2.StartAttack();

    if (!alreadyHit2 && tocando)
    {
        int damage = 7 + rand() % 2;
        player1.TakeDamage(damage);

        std::cout
            << "Jugador 2 golpea. Vida P1 = "
            << player1.GetHealth()
            << std::endl;

        alreadyHit2 = true;
    }
}
else
{
    alreadyHit2 = false;
    player2.StopAttack();
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

                int tiempo = tiempoCongelado;
                tiempo = 60 - reloj.getElapsedTime().asSeconds();

                if(tiempo < 0)
                    tiempo = 0;

                tiempoCongelado = tiempo;

                if(tiempo == 0 || player1.GetHealth() <= 0 || player2.GetHealth() <= 0)
                { 
                    peleaTerminada = true;

                    if(player2.GetHealth() <= 0 || (tiempo == 0 && player1.GetHealth() > player2.GetHealth()))
                    {
                        rondasGanadasP1++;
                        textoFinal.setString("JUGADOR UNO GANÓ ESTA RONDA\n    [Presiona Enter para continuar]");
                    }
                    else if(player1.GetHealth() <= 0 || (tiempo == 0 && player2.GetHealth() > player1.GetHealth()))
                    {
                        rondasGanadasP2++;
                        textoFinal.setString("JUGADOR DOS GANÓ ESTA RONDA\n    [Presiona Enter para continuar]");
                    }
                    else
                    {
                        textoFinal.setString("TIEMPO AGOTADO\n    [Presiona Enter para continuar]");
                    }

                    if (rondaActual == 3)
                    {
                        fightMusic.stop();

                        if(rondasGanadasP1 > rondasGanadasP2)
                        {
                            textoFinal.setString("JUGADOR UNO GANA\n[Presiona Enter para salir a la portada]");
                        }
                        else if(rondasGanadasP2 > rondasGanadasP1)
                        {
                            textoFinal.setString("JUGADOR DOS GANA\n[Presiona Enter para salir a la portada]");
                        }
                        else 
                        {
                            if(rondasGanadasP1 == 0 && rondasGanadasP2 == 0)
                            {
                                textoFinal.setString("EMPATE\n[Presiona Enter para salir a la portada]");
                            }
                            else
                            {
                                textoFinal.setString("TIEMPO AGOTADO\n[Presiona Enter para salir a la portada]");
                            }
                        }
                    }

        sf::FloatRect textBounds = textoFinal.getLocalBounds();
        
textoFinal.setOrigin({
    textBounds.position.x + textBounds.size.x * 0.5f,
    textBounds.position.y + textBounds.size.y * 0.5f
});

sf::Vector2u winSize = window.getSize();

textoFinal.setPosition({
    winSize.x * 0.5f,
    winSize.y * 0.5f
});

                    bool esperarEnter = true;
                    while(window.isOpen() && esperarEnter)
                    {
                        while(auto event = window.pollEvent())
                        {
                            if(event->is<sf::Event::Closed>())
                            {
                                window.close();
                                return 0;
                            }

                            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                            {
                                if (keyPressed->scancode == sf::Keyboard::Scancode::Enter)
                                {
                                    esperarEnter = false;
                                }
                            }
                        }

                        window.clear();
                        mapManager.draw(window);

                        window.draw(player1.GetSprite());
                        window.draw(player2.GetSprite());

                        textoVida1.setString("P1 Rondas: " + std::to_string(rondasGanadasP1) + " | HP: " + std::to_string(player1.GetHealth()));
                        textoVida2.setString("P2 Rondas: " + std::to_string(rondasGanadasP2) + " | HP: " + std::to_string(player2.GetHealth()));
                        textoTiempo.setString("FIN RONDA " + std::to_string(rondaActual));

                        window.draw(textoVida1);
                        window.draw(textoVida2);
                        window.draw(textoTiempo);
                        window.draw(textoFinal);

                        window.display();
                    }
                }

                if(!peleaTerminada)
                {
                    textoVida1.setString("P1 Rondas: " + std::to_string(rondasGanadasP1) + " | HP: " + std::to_string(player1.GetHealth()));
                    textoVida2.setString("P2 Rondas: " + std::to_string(rondasGanadasP2) + " | HP: " + std::to_string(player2.GetHealth()));
                    textoTiempo.setString("Ronda " + std::to_string(rondaActual) + " - " + std::to_string(tiempo));

                    window.clear();
                    mapManager.draw(window);

                    window.draw(player1.GetSprite());
                    window.draw(player2.GetSprite());

                    window.draw(textoVida1);
                    window.draw(textoVida2);
                    window.draw(textoTiempo);

                    window.display();
                }
            }

            rondaActual++;
        }

        musicStarted = false;
    }

    return 0;
}
