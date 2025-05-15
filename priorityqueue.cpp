#include "priorityqueue.h"

bool operator< (QPoint one, QPoint two) { //QPoint не имеет стандартного оператора< ; пришлось писать его самостоятельно, требуется для сравнения QPair
    if (one.x() * one.x() + one.y() * one.y() < two.x() * two.x() + two.y() * two.y()) {
        return true;
    }
    return false;
}

PriorityQueue::PriorityQueue() {

}

void PriorityQueue::push_back(QPair<int, QPoint> elem) {
    QVector::push_back(elem);
    place_elem(); //после стандартного добавления элемента вызывается функция размещения его на правильное место в отсортированном по возрастанию векторе
}

void PriorityQueue::place_elem() {
    int currIndex = size() - 1;
    QPair<int, QPoint> curr = last();

    int iterIndex = size() - 2;
    iterator iter = end()-2;
    while (curr.first < (*iter).first) { //идем по вектору и перемещаем элемент влево пока он не окажется на законном месте
        swapItemsAt(currIndex, iterIndex); //что немаловажно, элементы с одинаковыми первыми аргументами (в нашем случае стоимостями) не переписывают друг друга
        currIndex = iterIndex;
        iterIndex--;
        iter--;
        if (curr == *begin()) //во избежание выхода за пределы вектора
            break;
    }
}
