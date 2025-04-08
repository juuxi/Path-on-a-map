#ifndef MAP_H
#define MAP_H

#include <QPoint>
#include <QVector>
#include <QMap>

struct Obstacle {
    QVector<QPoint> points;
    int resistance;
    Obstacle(QVector<QPoint> _points, int _resistance) : points(_points), resistance(_resistance) {}
};

class Map
{
    QPoint start;
    QPoint finish;
    int width;
    int height;
    QVector<Obstacle*> obstacles;
    int Heuristic(QPoint, QPoint);
public:
    Map();
    int FindPath();
    void AddObstacle(Obstacle*);
    void RemoveObstacle(Obstacle*);
    void SetStart(QPoint);
    void SetFinish(QPoint);
    ~Map();
};

#endif // MAP_H
