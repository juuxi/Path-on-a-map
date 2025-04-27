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
    insertionSort(0, size());
}

void PriorityQueue::insertionSort(int left, int right) {
    for(size_t i = left+1; i < right; i++)
    {
        QPair<int, QPoint> value = operator [](i);
        for (int j = i-1; j >= left && value < operator [](j); j--)
        {
            QPair<int, QPoint> temp = operator [](j);
            operator [](j) = operator [](j+1);
            operator [](j+1) = temp;
        }
    }
}

int PriorityQueue::getMinrun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

void PriorityQueue::merge(int left, int mid, int right) {
    int size1 = mid - left + 1;
    int size2 = right - mid;

    // Создаем временные массивы
    QVector<QPair<int, QPoint>> leftArr(size1);
    QVector<QPair<int, QPoint>> rightArr(size2);

    // Копируем данные во временные массивы
    for (int i = 0; i < size1; i++)
        leftArr[i] = operator[] (left+i);
    for (int j = 0; j < size2; j++)
        rightArr[j] = operator[] (mid+1+j);

    int i = 0, j = 0, k = left;
    int count = 0;  // Счетчик для отслеживания количества выборок из одного подмассива
    bool fromLeft = true;  // Откуда брали последний элемент
    while (i < size1 && j < size2) {
        if (leftArr[i] <= rightArr[j]) {
            operator[] (k++) = leftArr[i++];
            count++;
            fromLeft = true;
        }
        else {
            operator[] (k++) = rightArr[j++];
            count++;
            fromLeft = false;
        }

        // Проверяем, достигли ли мы 7 выборок из одного подмассива
        if (count >= 7) {
            if (fromLeft) {
                // Переходим в режим "галоп" для правого подмассива
                QPair<int, QPoint> target = leftArr[i - 1];
                // Бинарный поиск для нахождения следующего элемента из правого подмассива
                while (j < size2 && rightArr[j] < target) {
                    operator[] (k++) = rightArr[j++];
                }
                count = 0;  // Сбрасываем счетчик
            }
            else {
                // Переходим в режим "галоп" для левого подмассива
                QPair<int, QPoint> target = rightArr[j - 1];
                // Бинарный поиск для нахождения следующего элемента из левого подмассива
                while (i < size1 && leftArr[i] < target) {
                    operator[] (k++) = leftArr[i++];
                }
                count = 0;  // Сбрасываем счетчик
            }
        }
    }

    // Копируем оставшиеся элементы из левого подмассива
    while (i < size1) {
        operator[] (k++) = leftArr[i++];
    }

    // Копируем оставшиеся элементы из правого подмассива
    while (j < size2) {
        operator[] (k++) = rightArr[j++];
    }
}

void PriorityQueue::TimSort() {
    int n = size();
    int minrun = getMinrun(n);

    // Сортируем подмассивы
    for (int start = 0; start < n; ) {
        // Находим конец текущего run
        int end = start;
        while (end + 1 < n && operator[] (end) <= operator[] (end+1)) {
            end++;
        }

        // Проверяем, упорядочен ли run по убыванию
        if (end > start && operator[] (end) < operator[] (end-1)) {
            std::reverse(begin() + start, begin() + end + 1);
        }

        // Если размер run меньше minrun, добавляем элементы
        int run_size = end - start + 1;
        if (run_size < minrun) {
            end = std::min(start + minrun - 1, n - 1);
        }

        // Сортируем текущий run с помощью сортировки вставками
        insertionSort(start, end);

        // Переходим к следующему run
        start = end + 1;
    }

    // Слияние подмассивов
    for (int size = minrun; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = std::min(left + size - 1, n - 1);
            int right = std::min(left + 2 * size - 1, n - 1);

            if (mid < right) {
                merge(left, mid, right);
            }
        }
    }
}

