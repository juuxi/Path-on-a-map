#include "map.h"
#include <math.h>

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

int Map::Heuristic(QPoint a, QPoint b) { //distance between two points
    return abs(a.x()-b.x()) + abs(a.y()-b.y());
}

int Map::FindPath() {
    QMap<int, QPoint> priority_queue; //map sorts by first argument,
    priority_queue.insert(0, start); //makes function first() very helpful
    QVector<QVector<QPoint>> came_from(500, QVector<QPoint>(500));//[x,y]
    QVector<QVector<int>> cost(500, QVector<int>(500));
    for (int i = 0; i < 500; i++) {
        for (int j = 0; j < 500; j++) {
            cost[i][j] = 0;
        }
    }
    //500 must be replaces by sizes of the window
    came_from[start.x()][start.y()] = start; //or null
    cost[start.x()][start.y()] = 0; //before stepping into curr
    while (!priority_queue.empty()) {
        QPoint curr = priority_queue.first();
        priority_queue.remove(priority_queue.firstKey());
        QVector<QPoint> neighbors;
        neighbors.push_back(QPoint(curr.x()-1, curr.y()));
        neighbors.push_back(QPoint(curr.x()+1, curr.y()));
        neighbors.push_back(QPoint(curr.x(), curr.y()-1));
        neighbors.push_back(QPoint(curr.x(), curr.y()+1));

        if (curr == finish) {
            break;
        }

        for (QPoint neighbor: neighbors) {
            int new_cost = cost[curr.x()][curr.y()] + 1; //1 must be replaced with cost
            //of moving from curr to neighbor
            if (cost[neighbor.x()][neighbor.y()] == 0 || new_cost < cost[neighbor.x()][neighbor.y()]) {
                int priority = new_cost + Heuristic(finish, neighbor);
                priority_queue.insert(priority, neighbor);
                cost[neighbor.x()][neighbor.y()] = new_cost;
                came_from[neighbor.x()][neighbor.y()] = curr;
            }
        }
    }
    return cost[finish.x()][finish.y()];
}

Map::~Map() {
    while (!obstacles.empty()) {
        delete obstacles.front();
        obstacles.pop_front();
    }
}
