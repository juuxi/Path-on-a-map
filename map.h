#ifndef MAP_H
#define MAP_H

#include <QPoint>
#include <QVector>

struct Obstacle {
    QVector<QPoint> points;
    int resistance;
};

class Map
{
    QPoint start;
    QPoint finish;
    int width;
    int height;
    QVector<Obstacle*> obstacles;
public:
    Map();
    void FindPath();
    void AddObstacle(Obstacle*);
    void RemoveObstacle(Obstacle*);
    void SetStart(QPoint);
    void SetFinish(QPoint);
    ~Map();
};

#endif // MAP_H
