#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QVector>
#include <QPoint>

class PriorityQueue : public QVector<QPair<int, QPoint>>
{
public:
    PriorityQueue();
    void sort();
    void push_back(QPair<int, QPoint>);
};

#endif // PRIORITYQUEUE_H
