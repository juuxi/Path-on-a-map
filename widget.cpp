#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    setupUI();
}

void Widget::setupUI() {
    setWindowTitle("Поиск кратчайшего пути на карте");
    setMinimumSize(500, 500);

    add_obstacle_btn = new QPushButton("Добавить объект", this);
    add_obstacle_btn->setFixedSize(150, 50);
    connect(add_obstacle_btn, &QPushButton::clicked, this, &Widget::add_obstacle_clicked);

    delete_obstacle_btn = new QPushButton("Удалить объект", this);
    delete_obstacle_btn->setFixedSize(150, 50);
    connect(delete_obstacle_btn, &QPushButton::clicked, this, &Widget::delete_obstacle_clicked);

    start_btn = new QPushButton("Старт", this);
    start_btn->setFixedSize(150, 50);
    connect(start_btn, &QPushButton::clicked, this, &Widget::start_clicked);

    finish_btn = new QPushButton("Финиш", this);
    finish_btn->setFixedSize(150, 50);
    connect(finish_btn, &QPushButton::clicked, this, &Widget::finish_clicked);

    execute_btn = new QPushButton("Выполнить", this);
    execute_btn->setFixedSize(150, 50);
    connect(execute_btn, &QPushButton::clicked, this, &Widget::execute_clicked);

    debug = new QLabel("aaa", this);
    debug->setGeometry(300, 100, 100, 30);
    mpdt.start = QPoint(200,150);
    mpdt.finish = QPoint(400,400);
    mpdt.height = 500;
    mpdt.width = 500;
    mpdt.left_map_margin = 150 + 2 * left_margin;

    start_point = new QLabel(this);
    start_point->setGeometry(200, 30, 125, 30);
    QString s;
    s += "Финиш: ";
    s += QString::number(mpdt.start.x());
    s += ",";
    s += QString::number(mpdt.start.y());
    start_point->setText(s);

    finish_point = new QLabel(this);
    finish_point->setGeometry(350, 30, 125, 30);
    s.clear();
    s += "Старт: ";
    s += QString::number(mpdt.finish.x());
    s += ",";
    s += QString::number(mpdt.finish.y());
    finish_point->setText(s);


    reposition_buttons();
}

void Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    mpdt.width = width();
    mpdt.height = height();
    reposition_buttons();
}

void Widget::reposition_buttons() {
    int windowHeight = height();
    int availableHeight = windowHeight - top_margin - bottom_margin;
    int totalButtonHeight = 6 * 50;
    int spacing = (availableHeight - totalButtonHeight) / (6 + 1);
    int topOffset = top_margin + spacing;

    add_obstacle_btn->move(left_margin, topOffset);
    topOffset += add_obstacle_btn->height() + spacing;

    delete_obstacle_btn->move(left_margin, topOffset);
    topOffset += delete_obstacle_btn->height() + spacing;

    start_btn->move(left_margin, topOffset);
    topOffset += start_btn->height() + spacing;

    finish_btn->move(left_margin, topOffset);
    topOffset += finish_btn->height() + spacing;

    execute_btn->move(left_margin, topOffset);
}

void Widget::add_obstacle_clicked() {
    is_adding_obstacle = true;
}

void Widget::delete_obstacle_clicked() {
    is_deleting_obstacle = true;
}

void Widget::start_clicked() {
    bool ok{};
    int new_x = QInputDialog::getInt(this, tr("Установить старт"),
                                         tr("x:"), 0, mpdt.left_map_margin, mpdt.width, 1, &ok);
    if (ok) {
        mpdt.start.setX(new_x);
    }
    int new_y = QInputDialog::getInt(this, tr("Установить старт"),
                                     tr("y:"), 0, 0, mpdt.height, 1, &ok);
    if (ok) {
        mpdt.start.setY(new_y);
    }
    QString s;
    s += "Финиш: ";
    s += QString::number(mpdt.start.x());
    s += ",";
    s += QString::number(mpdt.start.y());
    start_point->setText(s);
    update();
}

void Widget::finish_clicked() {
    bool ok{};
    int new_x = QInputDialog::getInt(this, tr("Установить финиш"),
                                     tr("x:"), 0, mpdt.left_map_margin, mpdt.width, 1, &ok);
    if (ok) {
        mpdt.finish.setX(new_x);
    }
    int new_y = QInputDialog::getInt(this, tr("Установить финиш"),
                                     tr("y:"), 0, 0, mpdt.height, 1, &ok);
    if (ok) {
        mpdt.finish.setY(new_y);
    }
    QString s;
    s += "Старт: ";
    s += QString::number(mpdt.finish.x());
    s += ",";
    s += QString::number(mpdt.finish.y());
    finish_point->setText(s);
    update();
}

void Widget::execute_clicked() {
    QString s;
    xml.WriteInFile("../../input.xml", mpdt);
    s += QString().number(map.FindPath());
    debug->setText(s);
    update();
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
    p.setBrush(QBrush(QColor(255, 0, 0)));
    p.drawEllipse(mpdt.start, 10, 10);
    p.setBrush(QBrush(QColor(0, 255, 0)));
    p.drawEllipse(mpdt.finish, 10, 10);
    map.PaintPath(&p);
    p.end();
}

void Widget::mousePressEvent(QMouseEvent* event) {
    if (is_adding_obstacle && event->button() == Qt::LeftButton) {
        new_poly_points.append(QCursor::pos());
    }
    if (is_deleting_obstacle && event->button() == Qt::LeftButton) {
        for(const Obstacle& obstacle: mpdt.obstacles) {
            QPolygon poly(obstacle.points);
            if(poly.containsPoint(QCursor::pos(), Qt::OddEvenFill)) {
                mpdt.obstacles.removeAt(mpdt.obstacles.indexOf(obstacle));
            }
        }
    }
    if (event->button() == Qt::RightButton) {
        is_adding_obstacle = false;
        is_deleting_obstacle = false;
        new_poly_points.clear();
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
        is_adding_obstacle = false;
        update();
    }
}

Widget::~Widget() {
}

