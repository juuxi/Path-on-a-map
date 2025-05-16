#ifndef MAP_H
#define MAP_H

#include <QMap>
#include <QPainter>
#include <priorityqueue.h>
#include "xml.h"
#include <QtMath>
#include <queue>

class Map
{
    Xml xml_processor; //интерфейс для работы с xml-файлами
    QPoint start;
    QPoint finish;
    int width;
    int height;
    int left_margin;
    QVector<QVector<QPoint>> came_from;
    QList<Obstacle> obstacles;
    float Heuristic(QPoint, QPoint); //функция подсчета абсолютного растояния между двумя точками
    QVector<QPoint> FindNeighbors(QPoint); //функция, возвращающая массив точек, в которые можно перейти из данной
    float CostMoving(QPoint, QPoint); //функция подсчета стоимости перемещения
    bool isOnCornerOfObject(QPoint);
public:
    Map();
    int FindPath(); //поиск пути
    void PaintPath(QPainter*); //отрисовка пути
    ~Map();
};

#endif // MAP_H
