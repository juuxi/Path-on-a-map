#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Поиск кратчайшего пути на карте");
    setFixedSize(1600, 800);
}

Widget::~Widget()
{
}

