#include "MapManager.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

MapManager::MapManager() : sprite(texture)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void MapManager::loadRandomMap()
{
    // 👉 Cambia aquí tus mapas si tienes más
    std::vector<std::string> maps =
    {
        "assets/imagenes/esenario viltrum.png"
        // puedes agregar más:
        // "assets/imagenes/mapa2.png",
        // "assets/imagenes/mapa3.png"
    };

    int index = std::rand() % maps.size();
    currentMap = maps[index];

    std::cout << "Mapa seleccionado: " << currentMap << std::endl;

    if (!texture.loadFromFile(currentMap))
    {
        std::cout << "ERROR: no se pudo cargar el mapa\n";
        return;
    }

    sprite.setTexture(texture);

sprite.setPosition({0.f, 0.f});
sprite.setScale({1.f, 1.f});

    std::cout << "Mapa cargado correctamente\n";
}

void MapManager::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

