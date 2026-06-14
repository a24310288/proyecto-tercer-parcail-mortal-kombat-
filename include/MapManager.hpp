#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MapManager {
private:
    // 🔥 REGLA DE ORO EN SFML 3: La textura DEBE declararse antes que el sprite
    sf::Texture texture;
    sf::Sprite sprite;
    std::string currentMap;

public:
    MapManager();
    void loadRandomMap();
    void draw(sf::RenderWindow& window);
};

#endif // MAP_MANAGER_HPP
