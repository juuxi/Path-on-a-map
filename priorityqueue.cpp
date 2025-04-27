#include "priorityqueue.h"

bool operator< (QPoint one, QPoint two) {
    if (one.x() * one.x() + one.y() * one.y() < two.x() * two.x() + two.y() * two.y()) {
        return true;
    }
    return false;
}

PriorityQueue::PriorityQueue() {

}

void PriorityQueue::push_back(QPair<int, QPoint> elem) {
    QVector::push_back(elem);
    place_elem();
}

void PriorityQueue::place_elem() {
    int currIndex = size() - 1;
    QPair<int, QPoint> curr = last();

    int iterIndex = size() - 2;
    iterator iter = end()-2;
    while (curr < *iter) {
        swapItemsAt(currIndex, iterIndex);
        currIndex = iterIndex;
        iterIndex--;
        iter--;
        if (curr == *begin())
            break;
    }
}
