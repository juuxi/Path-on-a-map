#include "priorityqueue.h"

bool operator< (QPoint one, QPoint two) {
    if (one.x() * one.x() + one.y() * two.y() < two.x() * two.x() + two.y() * two.y()) {
        return true;
    }
    return false;
}

PriorityQueue::PriorityQueue() {

}

void PriorityQueue::push_back(QPair<int, QPoint> elem) {
    QVector::push_back(elem);
    sort();
}

void PriorityQueue::sort() {
    for(size_t i = 1; i < size(); i++)
    {
        QPair<int, QPoint> value = operator [](i);
        for (int j = i-1; j >= 0 && value < operator [](j); j--)
        {
            QPair<int, QPoint> temp = operator [](j);
            operator [](j) = operator [](j+1);
            operator [](j+1) = temp;
        }
    }
}
