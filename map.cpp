#include "map.h"

Map::Map() {

}

Map::~Map() {
    while (!obstacles.empty()) {
        delete obstacles.front();
        obstacles.pop_front();
    }
}
