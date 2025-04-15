#include "map.h"
#include <math.h>

Map::Map() {
    width = 500;
    height = 500;
}

int Map::Heuristic(QPoint a, QPoint b) { //distance between two points
    return abs(a.x()-b.x()) + abs(a.y()-b.y());
}

QVector<QPoint> Map::FindNeighbors(QPoint p) {
    QVector<QPoint> neighbors;
    if (p.x() != 0)
        neighbors.push_back(QPoint(p.x()-1, p.y()));
    if (p.x() != width)
        neighbors.push_back(QPoint(p.x()+1, p.y()));
    if (p.y() != 0)
        neighbors.push_back(QPoint(p.x(), p.y()-1));
    if (p.y() != height)
        neighbors.push_back(QPoint(p.x(), p.y()+1));
    if (p.x() != 0 && p.y() != 0)
        neighbors.push_back(QPoint(p.x()-1, p.y()-1));
    if (p.x() != width && p.y() != 0)
        neighbors.push_back(QPoint(p.x()+1, p.y()-1));
    if (p.x() != 0 && p.y() != height)
        neighbors.push_back(QPoint(p.x()-1, p.y()+1));
    if (p.x() != width && p.y() != height)
        neighbors.push_back(QPoint(p.x()+1, p.y()+1));
    return neighbors;
}

int Map::FindPath() {
    MapData mpdt = xml_processor.ReadFile("../../input.xml");
    start = mpdt.start;
    finish = mpdt.finish;
    height = mpdt.height;
    width = mpdt.width;
    obstacles = mpdt.obstacles;
    QMap<int, QPoint> priority_queue; //map sorts by first argument,
    priority_queue.insert(0, start); //makes function first() very helpful
    QVector<QVector<QPoint>> came_from(500, QVector<QPoint>(500));//[x,y]
    QVector<QVector<int>> cost(width, QVector<int>(height));
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            cost[i][j] = 0;
        }
    }
    came_from[start.x()][start.y()] = start; //or null
    cost[start.x()][start.y()] = 0; //before stepping into curr
    while (!priority_queue.empty()) {
        QPoint curr = priority_queue.first();
        priority_queue.remove(priority_queue.firstKey());
        QVector<QPoint> neighbors = FindNeighbors(curr);

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
    QList<QPoint> test;
    test.push_back(QPoint(1,0));
    xml_processor.WriteOutFile("../../output.xml", mpdt, test, 1, 1);
    return cost[finish.x()][finish.y()];
}

Map::~Map() {

}
