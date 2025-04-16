#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
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
    mpdt.start = QPoint(50,50);
    mpdt.finish = QPoint(100,100);
    mpdt.height = 500;
    mpdt.width = 500;
    xml.WriteInFile("../../input.xml", mpdt);
    QString s;
    s += QString().number(map.FindPath());
    debug->setText(s);

    is_adding_obstacle = true;
}

void Widget::paintEvent(QPaintEvent*) {
    QPainter p;
    p.begin(this);
    p.setBrush(QBrush(QColor(120)));
    p.setPen(QPen(QColor(50)));
    for (int i = 1; i < new_poly_points.size(); i++) {
        p.drawLine(new_poly_points[i-1], new_poly_points[i]);
    }
    for (Obstacle obstacle: mpdt.obstacles) {
        QPolygon poly(obstacle.points);
        p.drawPolygon(poly);
    }
    p.end();
}

void Widget::mousePressEvent(QMouseEvent*) {
    if (is_adding_obstacle) {
        new_poly_points.append(QCursor::pos());
    }
    update();
}

void Widget::mouseDoubleClickEvent(QMouseEvent*) {
    if (is_adding_obstacle) {
        Obstacle to_add;
        to_add.points = new_poly_points;
        new_poly_points.clear();
        to_add.impassability = 50;
        mpdt.obstacles.push_back(to_add);
        xml.WriteInFile("../../input.xml", mpdt);
        update();
    }
}

Widget::~Widget() {
}

