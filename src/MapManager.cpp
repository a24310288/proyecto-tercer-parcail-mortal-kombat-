#include "MapManager.hpp"
#include <cstdlib>
#include <iostream>   // 👈 IMPORTANTE

MapManager::MapManager() : sprite(texture)
{
    loadRandomMap();
}


void MapManager::loadRandomMap()
{
    static std::vector<std::string> maps = {
        "assets/mapas/ecenario rengoku.png",
        "assets/mapas/esenario chavo.png",
        "assets/mapas/esenario kratos.png",
        "assets/mapas/esenario viltrum.png"
    };

    int index = std::rand() % maps.size();
    currentMap = maps[index];

    if (!texture.loadFromFile(currentMap)) {
        std::cout << "Error cargando mapa: " << currentMap << std::endl;
        return;
    }

    sprite.setTexture(texture);
}

void MapManager::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}
