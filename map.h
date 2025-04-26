#ifndef MAP_H
#define MAP_H

#include <QMap>
#include <QPainter>
#include <priorityqueue.h>
#include "xml.h"

class Map
{
    Xml xml_processor;
    QPoint start;
    QPoint finish;
    int width;
    int height;
    int left_margin;
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
