#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QVector>
#include <QPoint>
#include <algorithm>

class PriorityQueue : public QVector<QPair<int, QPoint>>
{
public:
    PriorityQueue();
    void place_elem(); //помещение элемента на правильное место
    void push_back(QPair<int, QPoint>); //переопределение добавления элемента в конец
};

#endif // PRIORITYQUEUE_H
