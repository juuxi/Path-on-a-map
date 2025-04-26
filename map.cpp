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
    if (p.x() != left_margin)
        neighbors.push_back(QPoint(p.x()-1, p.y()));
    if (p.x() != width)
        neighbors.push_back(QPoint(p.x()+1, p.y()));
    if (p.y() != left_margin)
        neighbors.push_back(QPoint(p.x(), p.y()-1));
    if (p.y() != height)
        neighbors.push_back(QPoint(p.x(), p.y()+1));
    if (p.x() != left_margin && p.y() != 0)
        neighbors.push_back(QPoint(p.x()-1, p.y()-1));
    if (p.x() != width && p.y() != 0)
        neighbors.push_back(QPoint(p.x()+1, p.y()-1));
    if (p.x() != left_margin && p.y() != height)
        neighbors.push_back(QPoint(p.x()-1, p.y()+1));
    if (p.x() != width && p.y() != height)
        neighbors.push_back(QPoint(p.x()+1, p.y()+1));
    return neighbors;
}

int Map::CostMoving(QPoint from, QPoint to) {
    for (Obstacle obstacle: obstacles) {
        QPolygon poly(obstacle.points);
        if(poly.containsPoint(to, Qt::OddEvenFill)) {
            return obstacle.impassability;
        }
    }
    return 1;
}

int Map::FindPath() {
    MapData mpdt = xml_processor.ReadFile("../../input.xml");
    start = mpdt.start;
    finish = mpdt.finish;
    height = mpdt.height;
    width = mpdt.width;
    obstacles = mpdt.obstacles;
    left_margin = mpdt.left_map_margin;
    QMap<int, QPoint> priority_queue; //map sorts by first argument,
    priority_queue.insert(0, start); //makes function first() very helpful
    QVector<QVector<int>> cost(width+1, QVector<int>(height+1));
    for (int i = 0; i < width+1; i++) {
        for (int j = 0; j < height+1; j++) {
            cost[i][j] = 0;
        }
    }
    came_from.resize(width+1);
    for (int i = 0; i < came_from.size(); i++) {
        came_from[i].resize(height+1);
    }
    came_from[start.x()][start.y()] = start; //or null
    cost[start.x()][start.y()] = 0; //before stepping into curr
    while (!priority_queue.empty()) {
        QPoint curr = priority_queue.first();
        priority_queue.remove(priority_queue.firstKey());
        if (curr == QPoint(201, 153))
            printf("b");
        QVector<QPoint> neighbors = FindNeighbors(curr);

        if (curr == finish) {
            break;
        }

        for (QPoint neighbor: neighbors) {
            if (neighbor.x() == 224)
                printf("a");
            int new_cost = cost[curr.x()][curr.y()] + CostMoving(curr, neighbor);
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
    QPoint curr = finish;
    while (curr != start) {
        QPoint from = came_from[curr.x()][curr.y()];
        test.push_front(curr);
        curr = from;
    }
    xml_processor.WriteOutFile("../../output.xml", mpdt, test, test.size(), cost[finish.x()][finish.y()]);

    return cost[finish.x()][finish.y()];
}

void Map::PaintPath(QPainter* p) {
    QPoint curr = finish;
    p->setPen(QPen(QColor(255, 0, 0)));
    while (curr != start) {
        QPoint from = came_from[curr.x()][curr.y()];
        p->drawLine(curr.x(), curr.y(), from.x(), from.y());
        curr = from;
    }
}

Map::~Map() {

}
