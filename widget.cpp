#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_executing = false;
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
    mpdt.start = QPoint(200,150);
    mpdt.finish = QPoint(400,400);
    mpdt.height = 500;
    mpdt.width = 500;
    mpdt.left_map_margin = 150 + 2 * left_margin;

    debug = new QLabel(this);
    debug->setGeometry(mpdt.left_map_margin, 10, 50, 30);

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
    int new_x = QInputDialog::getInt(this, tr("Старт"),
                                         tr("x:"), 0, mpdt.left_map_margin, mpdt.width, 1, &ok);
    if (ok) {
        mpdt.start.setX(new_x);
    }
    int new_y = QInputDialog::getInt(this, tr("Старт"),
                                     tr("y:"), 0, 0, mpdt.height, 1, &ok);
    if (ok) {
        mpdt.start.setY(new_y);
    }
    update();
}

void Widget::finish_clicked() {
    bool ok{};
    int new_x = QInputDialog::getInt(this, tr("Финиш"),
                                     tr("x:"), 0, mpdt.left_map_margin, mpdt.width, 1, &ok);
    if (ok) {
        mpdt.finish.setX(new_x);
    }
    int new_y = QInputDialog::getInt(this, tr("Финиш"),
                                     tr("y:"), 0, 0, mpdt.height, 1, &ok);
    if (ok) {
        mpdt.finish.setY(new_y);
    }
    update();
}

void Widget::execute_clicked() {
    xml.WriteInFile("../../input.xml", mpdt);
    map.FindPath();
    is_executing = true;
    update();
    PathData ptdt = xml.ReadOutFile("../../output.xml");
    QMessageBox msg_box;
    QString s;
    s += "Информация о пути:";
    s += "\n";
    s += "Дистанция: ";
    s += QString::number(ptdt.distance);
    s += "\n";
    s += "Время прохождения: ";
    s += QString::number(ptdt.time);
    msg_box.setText(s);
    msg_box.exec();
}

void Widget::paintEvent(QPaintEvent*) {
    QPainter p;
    p.begin(this);
    p.setPen(QPen(QColor(50)));
    for (int i = 1; i < new_poly_points.size(); i++) {
        p.drawLine(new_poly_points[i-1], new_poly_points[i]);
    }
    for (Obstacle obstacle: mpdt.obstacles) {
        QPolygon poly(obstacle.points);
        p.setBrush(QBrush(QColor(0, 0, 255 - 2 * obstacle.impassability)));
        p.drawPolygon(poly);
    }
    p.setBrush(QBrush(QColor(255, 0, 0)));
    p.drawEllipse(mpdt.start, 5, 5);
    p.setBrush(QBrush(QColor(0, 255, 0)));
    p.drawEllipse(mpdt.finish, 5, 5);
    if (is_executing) {
        map.PaintPath(&p);
        is_executing = false;
    }
    p.end();
}

void Widget::mousePressEvent(QMouseEvent* event) {
    if (is_adding_obstacle && event->button() == Qt::LeftButton) {
        if (QCursor::pos().x() >= mpdt.left_map_margin) {
            new_poly_points.append(QCursor::pos());
        }
        else {
            QMessageBox msg_box;
            QString s;
            s += "Нельзя ставить точку левее x=";
            s += QString::number(mpdt.left_map_margin);
            msg_box.setText(s);
            msg_box.exec();
        }
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
        to_add.impassability = QInputDialog::getInt(this, tr("Препятствие"),
                                         tr("Непроходимость:"), 0, 1, 100, 1, NULL);
        mpdt.obstacles.push_back(to_add);
        is_adding_obstacle = false;
        update();
    }
}

Widget::~Widget() {
    delete add_obstacle_btn;
    delete delete_obstacle_btn;
    delete start_btn;
    delete finish_btn;
    delete execute_btn;

    delete debug;
}

