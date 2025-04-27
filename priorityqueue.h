#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QVector>
#include <QPoint>
#include <algorithm>

class PriorityQueue : public QVector<QPair<int, QPoint>>
{
public:
    PriorityQueue();
    void place_elem();
    void push_back(QPair<int, QPoint>);
};

#endif // PRIORITYQUEUE_H
