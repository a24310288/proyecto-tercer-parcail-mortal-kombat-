#include "MapManager.hpp"
#include <iostream>
#include <cstdlib>

MapManager::MapManager() : sprite(texture)
{
}

void MapManager::loadRandomMap()
{
    static std::vector<std::string> maps = {
        "assets/imagenes/ecenario rengoku.png",
        "assets/imagenes/esenario chavo.png",
        "assets/imagenes/esenario kratos.png",
        "assets/imagenes/esenario viltrum.png"
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

