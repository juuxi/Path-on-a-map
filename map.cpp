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

void Map::SetStart(QPoint _start) {
    start = _start;
}

void Map::SetFinish(QPoint _finish) {
    finish = _finish;
}

void Map::FindPath() {
    QMap<int, QPoint> priority_queue; //map sorts by first argument,
    priority_queue.insert(0, start); //makes function first() very helpful
    QVector<QVector<int>> came_from(500, QVector<int>(500));
    QVector<QVector<int>> cost(500, QVector<int>(500));
    //500 must be replaces by sizes of the window
}

Map::~Map() {
    while (!obstacles.empty()) {
        delete obstacles.front();
        obstacles.pop_front();
    }
}
