#ifndef MAP_H
#define MAP_H

#include <QPoint>
#include <QMap>
#include <QPolygon>
#include "xml.h"

class Map
{
    Xml xml_processor;
    QPoint start;
    QPoint finish;
    int width;
    int height;
    QList<Obstacle> obstacles;
    int Heuristic(QPoint, QPoint);
    QVector<QPoint> FindNeighbors(QPoint);
    int CostMoving(QPoint, QPoint);
public:
    Map();
    int FindPath();
    ~Map();
};

#endif // MAP_H
