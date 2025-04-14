#ifndef MAP_H
#define MAP_H

#include <QPoint>
#include <QVector>
#include <QMap>
#include "xml.h"

class Map
{
    Xml xml_processor;
    QPoint start;
    QPoint finish;
    int width;
    int height;
    int Heuristic(QPoint, QPoint);
    QVector<QPoint> FindNeighbors(QPoint);
public:
    Map();
    QVector<Obstacle*> obstacles;
    int FindPath();
    void AddObstacle(Obstacle*);
    void RemoveObstacle(Obstacle*);
    void SetStart(QPoint);
    void SetFinish(QPoint);
    ~Map();
};

#endif // MAP_H
