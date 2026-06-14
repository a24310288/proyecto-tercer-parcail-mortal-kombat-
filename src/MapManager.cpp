#include "MapManager.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Enlazamos el sprite a la textura desde el nacimiento del objeto (Obligatorio en SFML 3)
MapManager::MapManager() : sprite(texture)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void MapManager::loadRandomMap()
{
    // Lista de tus escenarios disponibles
    std::vector<std::string> maps =
    {
        "assets/imagenes/esenario viltrum.png"
        // Si agregas más en el futuro, hazlo aquí separados por comas:
        // "assets/imagenes/mapa2.png",
        // "assets/imagenes/mapa3.png"
    };

    // Selección aleatoria del mapa
    int index = std::rand() % maps.size();
    currentMap = maps[index];

    std::cout << "Mapa seleccionado: " << currentMap << std::endl;

    // Intentar cargar la imagen en memoria
    if (!texture.loadFromFile(currentMap))
    {
        std::cout << "ERROR: No se pudo cargar el archivo: " << currentMap << "\n";
        return;
    }

    // Vincular la textura cargada al sprite
    sprite.setTexture(texture);

    // 🔥 PARCHE SFML 3: Forzamos al sprite a abrirse al tamaño real de la imagen
    sf::Vector2i tamanoImagen = { static_cast<int>(texture.getSize().x), static_cast<int>(texture.getSize().y) };
    sprite.setTextureRect(sf::IntRect({0, 0}, tamanoImagen));

    // Ajustar posición y escala usando la sintaxis moderna de llaves {}
    sprite.setPosition({0.f, 0.f});
    sprite.setScale({1.f, 1.f});

    std::cout << "Mapa cargado correctamente en el sprite.\n";
}

void MapManager::draw(sf::RenderWindow& window)
{
    // Renderizado del fondo en la ventana activa
    window.draw(sprite);
}

