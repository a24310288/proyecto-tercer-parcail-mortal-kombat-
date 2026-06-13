#include "MapManager.hpp"
#include <iostream>
#include <cstdlib>

MapManager::MapManager() : sprite(texture)
{
}

void MapManager::loadRandomMap()
{
    static std::vector<std::string> maps = {
        "assets/imagenes/escenario rengoku.png",
        "assets/imagenes/escenario chavo.png",
        "assets/imagenes/escenario kratos.png",
        "assets/imagenes/escenario viltrum.png"
    };

    int index = std::rand() % maps.size();
    currentMap = maps[index];

    std::cout << "Mapa seleccionado: " << currentMap << std::endl;

    if (!texture.loadFromFile(currentMap))
    {
        std::cout << "Error cargando imagen: " << currentMap << std::endl;
        return;
    }

    setupSprite();
}

void MapManager::setupSprite()
{
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(0.f, 0.f));
}

void MapManager::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

