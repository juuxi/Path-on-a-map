#include "map.h"
#include <math.h>

Map::Map() {
    width = 500;
    height = 500;
}

int Map::Heuristic(QPoint a, QPoint b) {
    int dx = abs(a.x()-b.x());
    int dy = abs(a.y()-b.y());
    int minim = 0;
    if (dx > dy) {
        minim = dy;
    }
    else {
        minim = dx;
    }
    return int(dx + dy + (qSqrt(2) - 2) * minim);
}

QVector<QPoint> Map::FindNeighbors(QPoint p) {
    QVector<QPoint> neighbors;
    if (p.x() > left_margin)
        neighbors.push_back(QPoint(p.x()-1, p.y()));
    if (p.x() != width-1)
        neighbors.push_back(QPoint(p.x()+1, p.y()));
    if (p.y() > left_margin)
        neighbors.push_back(QPoint(p.x(), p.y()-1));
    if (p.y() != height-1)
        neighbors.push_back(QPoint(p.x(), p.y()+1));
    if (p.x() > left_margin && p.y() != 0)
        neighbors.push_back(QPoint(p.x()-1, p.y()-1));
    if (p.x() != width-1 && p.y() != 0)
        neighbors.push_back(QPoint(p.x()+1, p.y()-1));
    if (p.x() > left_margin && p.y() != height-1)
        neighbors.push_back(QPoint(p.x()-1, p.y()+1));
    if (p.x() != width-1 && p.y() != height-1)
        neighbors.push_back(QPoint(p.x()+1, p.y()+1));
    return neighbors;
}

float Map::CostMoving(QPoint from, QPoint to) {
    float multiplier;
    if (from.x() == to.x() || from.y() == to.y()) {
        multiplier = 1;
    }
    else {
        multiplier = qSqrt(2);
    }

    for (Obstacle obstacle: obstacles) { //проходим по всем существующим препятствиям
        QPolygon poly(obstacle.points);
        if(poly.containsPoint(to, Qt::OddEvenFill)) { //если точка попала в одно из них - возвращем его показатель непроходимости
            return obstacle.impassability * multiplier;
        }
    }
    return 1 * multiplier; //если не попала - возвращаем единицу
}

int Map::FindPath() { //выбранный алгоритм - A*
    MapData mpdt = xml_processor.ReadInFile("../../input.xml"); //загружаем входные данные
    start = mpdt.start;
    finish = mpdt.finish;
    height = mpdt.height;
    width = mpdt.width;
    obstacles = mpdt.obstacles;
    left_margin = mpdt.left_map_margin;
    PriorityQueue priority_queue; //приоритетная очередь, которая будет сортировать элемент по "стоимости" (вектор cost)
    priority_queue.push_back(QPair<int, QPoint>(0, start)); //first - стоимость, second - точка
    QVector<QVector<float>> cost(width+1, QVector<float>(height+1)); //создаем вектора нужной длины
    for (int i = 0; i < width+1; i++) {
        for (int j = 0; j < height+1; j++) {
            cost[i][j] = 0;
        }
    }
    came_from.resize(width+1);
    for (int i = 0; i < came_from.size(); i++) {
        came_from[i].resize(height+1);
    }
    came_from[start.x()][start.y()] = start; //для старта устанавливаем "пришел из" на точку, опять же, старта; старт пришел "из ниоткуда"
    cost[start.x()][start.y()] = 0;
    while (!priority_queue.empty()) {
        QPoint curr = priority_queue.front().second;
        priority_queue.pop_front();
        QVector<QPoint> neighbors = FindNeighbors(curr);

        if (curr == finish) {
            break;
        }

        //DEBUG
        if (curr.x() == 281 && curr.y() == 355)
            printf("a");
        //DEBUG

        for (QPoint neighbor: neighbors) { //проходим по всем соседям данной точки
            float new_cost = cost[curr.x()][curr.y()] + CostMoving(curr, neighbor);
            if (cost[neighbor.x()][neighbor.y()] == 0 || new_cost < cost[neighbor.x()][neighbor.y()]) { //в случае выполнения хотя бы одного условия
                float priority = new_cost + Heuristic(finish, neighbor);
                priority_queue.push_back(QPair<float, QPoint>(priority, neighbor));
                cost[neighbor.x()][neighbor.y()] = new_cost; //обновляем стоимость
                came_from[neighbor.x()][neighbor.y()] = curr; //обновляем "пришел из"
            }
        }
    }

    QList<QPoint> test; //все точки, содержащиеся в пути
    QPoint curr = finish;
    while (curr != start) {
        QPoint from = came_from[curr.x()][curr.y()];
        test.push_front(curr);
        curr = from;
    }
    xml_processor.WriteOutFile("../../output.xml", mpdt, test, test.size(), int(cost[finish.x()][finish.y()])); //финальная стоимость отражает "время"

    return int(cost[finish.x()][finish.y()]);
}

void Map::PaintPath(QPainter* p) {
    QPoint curr = finish;
    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(2);
    p->setPen(pen);
    while (curr != start) { //рисуем путь прерывистой линией
        for (int i = 0; i < 6; i++) {
            QPoint from = came_from[curr.x()][curr.y()];
            p->drawLine(curr.x(), curr.y(), from.x(), from.y());
            curr = from;
            if (curr == start)
                break;
        }
        for (int i = 0; i < 4; i++) {
            QPoint from = came_from[curr.x()][curr.y()];
            curr = from;
            if (curr == start)
                break;
        }
    }
}

Map::~Map() {

}
