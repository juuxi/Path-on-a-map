#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Поиск кратчайшего пути на карте");
    setFixedSize(500, 500);
    start_point = new QLabel(this);
    //start_point->setGeometry(100, 100, 360, 360);
    QPixmap start_pic = QPixmap("../../start_point.jpg");
    start_point->setPixmap(start_pic);
    start_point->setMask(start_pic.mask());
    start_point->show();

    debug = new QLabel("aaa", this);
    debug->setGeometry(0, 100, 500, 360);
    map.SetFinish(QPoint(100,100));
    map.SetStart(QPoint(50,50));
    QString s;
    s += QString().number(map.FindPath());
    debug->setText(QDir::currentPath());
}

Widget::~Widget()
{
}

