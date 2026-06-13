#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class MapManager {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::string currentMap;

public:
    MapManager(); // 👈 ESTO ES LO QUE FALTA O ESTÁ MAL

    void loadRandomMap();
    void draw(sf::RenderWindow& window);
};
