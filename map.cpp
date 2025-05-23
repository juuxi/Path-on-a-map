#include "map.h"
#include <math.h>

struct Compare {
    bool operator()(const QPair<float, QPoint>& a, const QPair<float, QPoint>& b) {
        return a.first > b.first;
    }
};

Map::Map() {
    width = 500;
    height = 500;
    diagonal_steps_counter = 0;
}

float Map::Heuristic(QPoint a, QPoint b) {
    float dx = abs(a.x()-b.x());
    float dy = abs(a.y()-b.y());
    float minim = 0;
    if (dx > dy) {
        minim = dy;
    }
    else {
        minim = dx;
    }
    return dx + dy + (qSqrt(2) - 2) * minim;
}

QVector<QPoint> Map::FindNeighbors(QPoint p) {
    QVector<QPoint> neighbors;
    if (p.x() > left_margin)
        neighbors.push_back(QPoint(p.x()-1, p.y()));
    if (p.x() != width-1)
        neighbors.push_back(QPoint(p.x()+1, p.y()));
    if (p.y() > 0)
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

bool Map::isObstaclePoint(QPoint p) {
    for (Obstacle obs : obstacles) {
        if (obs.points.contains(p)) {
            return true;
        }
    }
    return false;
}

bool Map::isOnCornerOfObstacle(QPoint p) {

    int count = 0;
    if (p.x() > left_margin && isObstaclePoint(QPoint(p.x()-1, p.y())))
        count++;
    if (p.x() != width-1 && isObstaclePoint(QPoint(p.x()+1, p.y())))
        count++;
    if (p.y() > 0 && isObstaclePoint(QPoint(p.x(), p.y()-1)))
        count++;
    if (p.y() != height-1 && isObstaclePoint(QPoint(p.x(), p.y()+1)))
        count++;
    if (p.x() > left_margin && p.y() != 0 && isObstaclePoint(QPoint(p.x()-1, p.y()-1)))
        count++;
    if (p.x() != width-1 && p.y() != 0 && isObstaclePoint(QPoint(p.x()+1, p.y()-1)))
        count++;
    if (p.x() > left_margin && p.y() != height-1 && isObstaclePoint(QPoint(p.x()-1, p.y()+1)))
        count++;
    if (p.x() != width-1 && p.y() != height-1 && isObstaclePoint(QPoint(p.x()+1, p.y()+1)))
        count++;

    if (count > 0) { //count > 0 не лучше
        return true;
    }
    return false;
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
        if(poly.containsPoint(to, Qt::OddEvenFill) || poly.contains(to)) { //если точка попала в одно из них - возвращем его показатель непроходимости
            return obstacle.impassability * multiplier;
        }
    }
    return 1 * multiplier; //если не попала - возвращаем единицу
}

int Map::FindPath() { //выбранный алгоритм - A*
    diagonal_steps_counter = 0;
    MapData mpdt = xml_processor.ReadInFile("../../input.xml"); //загружаем входные данные
    start = mpdt.start;
    finish = mpdt.finish;
    height = mpdt.height;
    width = mpdt.width;
    obstacles = mpdt.obstacles;
    left_margin = mpdt.left_map_margin;
    //PriorityQueue priority_queue; //приоритетная очередь, которая будет сортировать элемент по "стоимости" (вектор cost)
    std::priority_queue<QPair<float, QPoint>, std::vector<QPair<float, QPoint>>, Compare> priority_queue;
    priority_queue.push(QPair<float, QPoint>(0, start)); //first - стоимость, second - точка
    QVector<QVector<float>> cost(width+1, QVector<float>(height+1, std::numeric_limits<float>::max()));//создаем вектора нужной длины
    cost[start.x()][start.y()] = 0;
    came_from.resize(width+1);
    for (int i = 0; i < came_from.size(); i++) {
        came_from[i].resize(height+1);
    }
    came_from[start.x()][start.y()] = start; //для старта устанавливаем "пришел из" на точку, опять же, старта; старт пришел "из ниоткуда"
    cost[start.x()][start.y()] = 0;
    while (!priority_queue.empty()) {
        QPoint curr = priority_queue.top().second;
        priority_queue.pop();
        QVector<QPoint> neighbors = FindNeighbors(curr);

        if (curr == finish) {
            break;
        }

        for (QPoint neighbor: neighbors) { //проходим по всем соседям данной точки
            float new_cost = cost[curr.x()][curr.y()] + CostMoving(curr, neighbor);
            if (new_cost < cost[neighbor.x()][neighbor.y()]) { //в случае выполнения хотя бы одного условия
                float priority = new_cost + Heuristic(finish, neighbor);
                priority_queue.push(QPair<float, QPoint>(priority, neighbor));
                cost[neighbor.x()][neighbor.y()] = new_cost; //обновляем стоимость
                came_from[neighbor.x()][neighbor.y()] = curr; //обновляем "пришел из"
            }
        }
    }

    QList<QPoint> test; //все точки, содержащиеся в пути
    QPoint curr = finish;
    while (curr != start) {
        QPoint from = came_from[curr.x()][curr.y()];
        if (from.x() != curr.x() && from.y() != curr.y()) {
            diagonal_steps_counter++;
        }
        test.push_front(curr);
        curr = from;
    }
    float distance = (diagonal_steps_counter * qSqrt(2)) + (test.size() - diagonal_steps_counter);
    xml_processor.WriteOutFile("../../output.xml", mpdt, test, int(distance), int(cost[finish.x()][finish.y()])); //финальная стоимость отражает "время"

    return int(cost[finish.x()][finish.y()]);
}

void Map::PaintPath(QPainter* p) {
    QPoint curr = finish;
    QPen pen;
    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(3);
    pen.setStyle(Qt::DashLine);
    p->setPen(pen);
    QPoint draw_from = finish;
    while (curr != start) { //рисуем путь прерывистой линией
        QPoint from = came_from[curr.x()][curr.y()];
        if (from == start || isOnCornerOfObstacle(from)) {
            p->drawLine(curr, draw_from);
            draw_from = curr;
        }
        if (curr == start) {
            break;
        }
        curr = from;
    }
}

Map::~Map() {

}
