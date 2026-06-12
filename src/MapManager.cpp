#include "MapManager.hpp"
#include <vector>
#include <cstdlib>

std::string MapManager::getRandomMap()
{
    static std::vector<std::string> maps = {
        "assets/mapas/ecenario rengoku.png",
        "assets/mapas/esenario chavo.png",
        "assets/mapas/esenario kratos.png",
        "assets/mapas/esenario omniman.png"
    };
    

    int index = std::rand() % maps.size();
    return maps[index];
}