#ifndef MAP_H
#define MAP_H

#include <QPoint>
#include <QMap>
#include <QPainter>
#include "xml.h"

class Map
{
    Xml xml_processor;
    QPoint start;
    QPoint finish;
    int width;
    int height;
    QVector<QVector<QPoint>> came_from;
    QList<Obstacle> obstacles;
    int Heuristic(QPoint, QPoint);
    QVector<QPoint> FindNeighbors(QPoint);
    int CostMoving(QPoint, QPoint);
public:
    Map();
    int FindPath();
    void PaintPath(QPainter*);
    ~Map();
};

#endif // MAP_H
