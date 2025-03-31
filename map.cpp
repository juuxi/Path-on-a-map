#include "map.h"

Map::Map() {

}

void Map::AddObstacle(Obstacle* _obstacle) {
    obstacles.push_back(_obstacle);
}

void Map::RemoveObstacle(Obstacle* _obstacle) {
    if (obstacles.indexOf(_obstacle) != -1) {
        obstacles.remove(obstacles.indexOf(_obstacle));
    }
}

Map::~Map() {
    while (!obstacles.empty()) {
        delete obstacles.front();
        obstacles.pop_front();
    }
}
