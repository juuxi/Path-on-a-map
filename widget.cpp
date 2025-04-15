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
    debug->setText(s);

    is_adding_obstacle = false;

    MapData mpdt;
    mpdt.start = QPoint(0,0);
    mpdt.finish = QPoint(100,100);
    mpdt.height = 500;
    mpdt.width = 500;
    QList<Obstacle> obss;
    Obstacle obs;
    obs.impassability = 10;
    QVector<QPoint> points;
    points.push_back(QPoint(1,1));
    obs.points = points;
    obss.push_back(obs);
    mpdt.obstacles = obss;
    xml.WriteInFile("../../input.xml", mpdt);
}

void Widget::paintEvent(QPaintEvent*) {
    QPainter p;
    p.begin(this);
    p.setBrush(QBrush(QColor(120)));
    QVector<QPoint> points = {QPoint(50, 100), QPoint(100, 50),
                              QPoint(150, 100), QPoint(100, 150)};
    QPolygon poly(points);
    for (int i = 0; i < map.obstacles.size(); i++) {
        QPolygon poly_2(map.obstacles[i]->points);
        p.drawPolygon(poly_2);
    }
    p.drawPolygon(poly);
    p.end();
}

void Widget::mousePressEvent(QMouseEvent*) {
    if (is_adding_obstacle) {
        new_poly_points.append(QCursor::pos());
    }
}

void Widget::mouseDoubleClickEvent(QMouseEvent*) {
    if (is_adding_obstacle) {
        Obstacle* to_add = new Obstacle;
        to_add->points = new_poly_points;
        new_poly_points.clear();
        to_add->impassability = 50;
        map.AddObstacle(to_add);
        update();
    }
}

Widget::~Widget()
{
}

