#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MapManager {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    std::string currentMap;

public:
    MapManager();
    void loadRandomMap();
    void draw(sf::RenderWindow& window);
};

#endif // MAP_MANAGER_HPP
