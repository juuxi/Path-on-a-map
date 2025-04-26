#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QVector>
#include <QPoint>

class PriorityQueue : public QVector<QPair<unsigned, QPoint>>
{
public:
    PriorityQueue();
    void sort();
};

#endif // PRIORITYQUEUE_H
