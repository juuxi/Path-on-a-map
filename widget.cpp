#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Поиск кратчайшего пути на карте");
    setFixedSize(1600, 800);
    start_point = new QLabel(this);
    start_point->setGeometry(100, 100, 360, 360);
    QPixmap start_pic = QPixmap("/home/juuxi/Desktop/prog/ETU/2nd_course/path_on_a_map/start_point.jpg");
    start_point->setPixmap(start_pic);
    start_point->setMask(start_pic.mask());
    start_point->show();
}

Widget::~Widget()
{
}

