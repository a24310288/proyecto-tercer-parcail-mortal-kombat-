#include "MapManager.hpp"
#include <cstdlib>
#include <iostream>

MapManager::MapManager() : sprite(texture)
{
    loadRandomMap();
}

void MapManager::setupSprite()
{
    sprite.setTexture(texture);

    // 🔥 Ajuste a pantalla (opcional pero recomendado)
    sprite.setScale(sf::Vector2f(
        1280.f / static_cast<float>(texture.getSize().x),
        720.f / static_cast<float>(texture.getSize().y)
    ));
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
        std::cout << "Error cargando mapa: " << currentMap << std::endl;
        return;
    }

    setupSprite();
}

void MapManager::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
