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
    std::vector<std::string> maps =
    {
        "assets/imagenes/esenario viltrum.png",
        "assets/imagenes/esenario chavo.png",
        "assets/imagenes/ecenario rengoku.png",
        "assets/imagenes/esenario kratos.png"

    };

    int index = std::rand() % maps.size();
    currentMap = maps[index];

    std::cout << "Mapa seleccionado: " << currentMap << std::endl;

    if (!texture.loadFromFile(currentMap))
    {
        std::cout << "ERROR: No se pudo cargar el archivo: " << currentMap << "\n";
        return;
    }

    sprite.setTexture(texture);

    sf::Vector2i tamanoImagen = { static_cast<int>(texture.getSize().x), static_cast<int>(texture.getSize().y) };
    sprite.setTextureRect(sf::IntRect({0, 0}, tamanoImagen));

    sprite.setPosition({0.f, 0.f});
    sprite.setScale({1.f, 1.f});

    std::cout << "Mapa cargado correctamente en el sprite.\n";
}

void MapManager::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

