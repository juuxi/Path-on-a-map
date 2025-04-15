#ifndef MAP_H
#define MAP_H

#include <QPoint>
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
    QList<Obstacle> obstacles;
    int FindPath();
    ~Map();
};

#endif // MAP_H
