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

// Función auxiliar para extraer el nombre limpio del personaje si se pasa una ruta de archivo
std::string obtenerNombreLimpio(const std::string& rutaOTexto)
{
    // Si contiene una barra o extensión, extraemos solo el nombre del archivo sin extensión
    if (rutaOTexto.find('/') != std::string::npos || rutaOTexto.find('\\') != std::string::npos)
    {
        std::filesystem::path p(rutaOTexto);
        return p.stem().string(); // "assets/imagenes/kratos.jpeg" -> se convierte en "kratos"
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

    // Elementos de la interfaz de texto
    sf::Text textoVida1(font);
    sf::Text textoVida2(font);
    sf::Text textoTiempo(font);
    sf::Text textoFinal(font); 

    textoVida1.setCharacterSize(30);
    textoVida2.setCharacterSize(30);
    textoTiempo.setCharacterSize(35);
    textoFinal.setCharacterSize(50); // Ajustado para que quepan textos más largos de ronda

    textoVida1.setFillColor(sf::Color::White);
    textoVida2.setFillColor(sf::Color::White);
    textoTiempo.setFillColor(sf::Color::Yellow);
    textoFinal.setFillColor(sf::Color::Red); 

    textoVida1.setPosition({20.f,20.f});
    textoVida2.setPosition({950.f,20.f}); // Movido un poco a la izquierda por los marcadores
    textoTiempo.setPosition({550.f,20.f});

    // BUCLE MAESTRO: Controla la rejugabilidad completa del juego
    while(window.isOpen())
    {
        // --- 1. PANTALLA DE PORTADA ---
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

        // --- 2. SELECCIÓN DE PERSONAJES ---
        CharacterSelect select;
        select.Run(window);

        // --- 3. CONFIGURACIÓN INICIAL DEL COMBATE GLOBALES ---
        MapManager mapManager;
        int rondaActual = 1;
        int rondasGanadasP1 = 0;
        int rondasGanadasP2 = 0;

        // Bucle estructurado de 3 rondas obligatorias
        while(rondaActual <= 3 && window.isOpen())
        {
            // Cambiar dinámicamente de mapa aleatorio al inicio de cada ronda
            mapManager.loadRandomMap();

            // Sanear los nombres obtenidos para eliminar rutas duplicadas como assets/imagenes/kratos.jpeg
            std::string p1Nombre = obtenerNombreLimpio(select.GetPlayer1());
            std::string p2Nombre = obtenerNombreLimpio(select.GetPlayer2());

            Fighter player1(200, 500, p1Nombre);
            Fighter player2(900, 500, p2Nombre);

            sf::Clock reloj;
            bool peleaTerminada = false;
            int tiempoCongelado = 60;

            // Loop activo de la pelea en la ronda actual
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

                // CONTROLES Y MOVIMIENTO
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

                // DETECCION DE DAÑO Y ATAQUES
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
                        int damage = 7 + rand() % 2;
                        player2.TakeDamage(damage);
                        std::cout << "P1 golpea -> -" << damage << " Vida P2: " << player2.GetHealth() << "\n";
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
                        int damage = 7 + rand() % 2;
                        player1.TakeDamage(damage);
                        std::cout << "P2 golpea -> -" << damage << " Vida P1: " << player1.GetHealth() << "\n";
                        alreadyHit2 = true;
                    }
                }
                else
                {
                    alreadyHit2 = false;
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

                // MANEJO DEL RELOJ DE RONDA
                int tiempo = tiempoCongelado;
                tiempo = 60 - reloj.getElapsedTime().asSeconds();

                if(tiempo < 0)
                    tiempo = 0;

                tiempoCongelado = tiempo;

                // --- DETECCION DE FIN DE RONDA ACTUAL ---
                if(tiempo == 0 || player1.GetHealth() <= 0 || player2.GetHealth() <= 0)
                { 
                    peleaTerminada = true;

                    // Evaluar ganador de la ronda individual
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

                    // --- EVALUACIÓN GLOBAL (Solo se gatilla al finalizar la ronda 3) ---
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
                            // Si se completaron las 3 rondas y las victorias globales quedaron en 0 a 0
                            if(rondasGanadasP1 == 0 && rondasGanadasP2 == 0)
                            {
                                textoFinal.setString("EMPATE\n[Presiona Enter para salir a la portada]");
                            }
                            else
                            {
                                // Empate global por puntos tras acabarse el tiempo general
                                textoFinal.setString("TIEMPO AGOTADO\n[Presiona Enter para salir a la portada]");
                            }
                        }
                    }

                    // Posicionar y centrar correctamente la etiqueta de texto final
                    sf::FloatRect textBounds = textoFinal.getLocalBounds();
                    textoFinal.setPosition({
                        (1280.f - textBounds.size.x) / 2.f,
                        (720.f - textBounds.size.y) / 2.f
                    });

                    // Bucle de bloqueo: Congela la escena para visualizar los textos y espera un "Enter"
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

                        // Renderizado estático mientras se espera la confirmación del usuario
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

                // Renderizado dinámico estándar del juego en curso
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

        // Reseteamos el flag de la música al salir de las 3 rondas para que vuelva a iniciar al ciclar a la portada
        musicStarted = false;
    }

    return 0;
}
