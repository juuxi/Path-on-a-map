#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QVector>
#include <QPoint>
#include <algorithm>

class PriorityQueue : public QVector<QPair<int, QPoint>>
{
public:
    PriorityQueue();
    void insertionSort(int, int);
    int getMinrun(int);
    void merge(int, int, int);
    void TimSort();
    void push_back(QPair<int, QPoint>);
};

#endif // PRIORITYQUEUE_H
