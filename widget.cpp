#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_executing = false;
    is_changing_start = false;
    is_changing_finish = false;
    SetupUI();
}

void Widget::SetupUI() {
    setWindowTitle("Поиск кратчайшего пути на карте");
    setMinimumSize(500, 500);

    add_obstacle_btn = new QPushButton("Добавить объект", this); //создание кнопок и свзяка их со слотами
    add_obstacle_btn->setFixedSize(150, 50);
    connect(add_obstacle_btn, &QPushButton::clicked, this, &Widget::AddObstacleClicked);

    delete_obstacle_btn = new QPushButton("Удалить объект", this);
    delete_obstacle_btn->setFixedSize(150, 50);
    connect(delete_obstacle_btn, &QPushButton::clicked, this, &Widget::DeleteObstacleClicked);

    start_btn = new QPushButton("Старт", this);
    start_btn->setFixedSize(150, 50);
    connect(start_btn, &QPushButton::clicked, this, &Widget::StartClicked);

    finish_btn = new QPushButton("Финиш", this);
    finish_btn->setFixedSize(150, 50);
    connect(finish_btn, &QPushButton::clicked, this, &Widget::FinishClicked);

    execute_btn = new QPushButton("Выполнить", this);
    execute_btn->setFixedSize(150, 50);
    connect(execute_btn, &QPushButton::clicked, this, &Widget::ExecuteClicked);

    save_btn = new QPushButton("Сохранить", this);
    save_btn->setFixedSize(150, 50);
    connect(save_btn, &QPushButton::clicked, this, &Widget::SaveClicked);

    mpdt.start = QPoint(200,150); //заполнение структуры MapData стандартными значениями
    mpdt.finish = QPoint(400,400);
    mpdt.height = 500;
    mpdt.width = 500;
    mpdt.left_map_margin = 150 + 2 * left_margin;

    //DEBUG
    Obstacle obs1;
    Obstacle obs2;
    QVector<QPoint> pt1;
    pt1.push_back(QPoint(305, 142));
    pt1.push_back(QPoint(197, 265));
    pt1.push_back(QPoint(239, 357));
    pt1.push_back(QPoint(422, 136));
    QVector<QPoint> pt2;
    pt2.push_back(QPoint(281, 355));
    pt2.push_back(QPoint(219, 394));
    pt2.push_back(QPoint(234, 438));
    pt2.push_back(QPoint(331, 375));
    obs1.points = pt1;
    obs1.impassability = 100;
    obs2.points = pt2;
    obs2.impassability = 100;
    QList<Obstacle> obss;
    obss.push_back(obs1);
    obss.push_back(obs2);
    mpdt.obstacles = obss;
    //DEBUG

    RepositionButtons();
}

void Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    mpdt.width = width();
    mpdt.height = height();
    RepositionButtons();
}

void Widget::RepositionButtons() {
    int window_height = height();
    int available_height = window_height - top_margin - bottom_margin;
    int total_button_height = 6 * 50;
    int spacing = (available_height - total_button_height) / (6 + 1);
    int top_offset = top_margin + spacing;

    add_obstacle_btn->move(left_margin, top_offset);
    top_offset += add_obstacle_btn->height() + spacing;

    delete_obstacle_btn->move(left_margin, top_offset);
    top_offset += delete_obstacle_btn->height() + spacing;

    start_btn->move(left_margin, top_offset);
    top_offset += start_btn->height() + spacing;

    finish_btn->move(left_margin, top_offset);
    top_offset += finish_btn->height() + spacing;

    execute_btn->move(left_margin, top_offset);
    top_offset += finish_btn->height() + spacing;

    save_btn->move(left_margin, top_offset);
}

void Widget::AddObstacleClicked() {
    is_adding_obstacle = true;

    is_deleting_obstacle = false;
    is_executing = false;
    is_changing_start = false;
    is_changing_finish = false;
}

void Widget::DeleteObstacleClicked() {
    is_deleting_obstacle = true;

    is_adding_obstacle = false;
    is_executing = false;
    is_changing_start = false;
    is_changing_finish = false;
}

void Widget::StartClicked() {
    is_changing_start = true;

    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_executing = false;
    is_changing_finish = false;
}

void Widget::FinishClicked() {
    is_changing_finish = true;

    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_executing = false;
    is_changing_start = false;
}

void Widget::ExecuteClicked() {
    map.FindPath(); //выполнение основного алгоритма
    is_executing = true;

    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_changing_start = false;
    is_changing_finish = false;

    update();
    PathData ptdt = xml.ReadOutFile("../../output.xml");
    mpdt.finish = ptdt.mpdt.finish;
    mpdt.start = ptdt.mpdt.start;
    mpdt.left_map_margin = ptdt.mpdt.left_map_margin;
    mpdt.obstacles = ptdt.mpdt.obstacles;
    QMessageBox msg_box; //вывод окна, сообщающего характеристики полученного пути
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

void Widget::SaveClicked() {
    xml.WriteInFile("../../input.xml", mpdt);
    is_adding_obstacle = false;
    is_deleting_obstacle = false;
    is_executing = false;
    is_changing_start = false;
    is_changing_finish = false;
}

void Widget::paintEvent(QPaintEvent*) {
    QPainter p;
    p.begin(this);
    p.setPen(QPen(QColor(50)));
    for (int i = 1; i < new_poly_points.size(); i++) { //отрисовка добавлемого препятствия при наличии такового
        p.drawLine(new_poly_points[i-1], new_poly_points[i]);
    }
    for (Obstacle obstacle: mpdt.obstacles) { //отрисовка существубщих препятствий
        QPolygon poly(obstacle.points);
        p.setBrush(QBrush(QColor(0, 0, 255 - 2 * obstacle.impassability))); //цвет зависит от показателя непроходимости: чем он выше - тем темнее цвет
        p.drawPolygon(poly);
    }
    p.setBrush(QBrush(QColor(255, 0, 0)));
    p.drawEllipse(mpdt.start, 5, 5); //отрисовка старта
    p.setBrush(QBrush(QColor(0, 255, 0)));
    p.drawEllipse(mpdt.finish, 5, 5); //отрисовка финиша
    if (is_executing) { //если была нажата кнопка "выполнить" - рисуем путь
        map.PaintPath(&p);
        is_executing = false;
    }
    p.end();
}

void Widget::mousePressEvent(QMouseEvent* event) {
    if (is_adding_obstacle && event->button() == Qt::LeftButton) { //если была нажата кнопка "добавление объекта", а после ЛКМ
        if (QCursor::pos().x() >= mpdt.left_map_margin) { //добавить точку если это можно сделать
            new_poly_points.append(QCursor::pos());
        }
        else { //вывести сообщение если нельзя
            QMessageBox msg_box;
            QString s;
            s += "Нельзя ставить точку левее x=";
            s += QString::number(mpdt.left_map_margin);
            msg_box.setText(s);
            msg_box.exec();
        }
    }
    else if (is_deleting_obstacle && event->button() == Qt::LeftButton) { //если была нажата кнопка "добавление объекта", а после ЛКМ
        for(const Obstacle& obstacle: mpdt.obstacles) { //найти объект, который хочет удалить пользователь и удалить его
            QPolygon poly(obstacle.points);
            if(poly.containsPoint(QCursor::pos(), Qt::OddEvenFill)) {
                mpdt.obstacles.removeAt(mpdt.obstacles.indexOf(obstacle));
            }
        }
    }
    else if (is_changing_start && event->button() == Qt::LeftButton) {
        if (QCursor::pos().x() >= mpdt.left_map_margin) { //добавить точку если это можно сделать
            mpdt.start = QCursor::pos();
        }
        else { //вывести сообщение если нельзя
            QMessageBox msg_box;
            QString s;
            s += "Нельзя ставить старт левее x=";
            s += QString::number(mpdt.left_map_margin);
            msg_box.setText(s);
            msg_box.exec();
        }
        is_changing_start = false;
    }
    else if (is_changing_finish && event->button() == Qt::LeftButton) {
        if (QCursor::pos().x() >= mpdt.left_map_margin) { //добавить точку если это можно сделать
            mpdt.finish = QCursor::pos();
        }
        else { //вывести сообщение если нельзя
            QMessageBox msg_box;
            QString s;
            s += "Нельзя ставить финиш левее x=";
            s += QString::number(mpdt.left_map_margin);
            msg_box.setText(s);
            msg_box.exec();
        }
        is_changing_finish = false;
    }
    else if (event->button() == Qt::RightButton) { //если была нажата ПКМ - "отменить" нажатие UI-кнопкок
        is_adding_obstacle = false;
        is_deleting_obstacle = false;
        is_executing = false;
        is_changing_start = false;
        is_changing_finish = false;
        new_poly_points.clear();
    }
    update();
}

void Widget::mouseDoubleClickEvent(QMouseEvent*) {
    if (is_adding_obstacle) { //если программа в состоянии добавления объекта - завершитб его добавление
        Obstacle to_add;
        to_add.points = new_poly_points;
        new_poly_points.clear();
        to_add.impassability = QInputDialog::getInt(this, tr("Препятствие"), //запрос значения показателя непроходимости
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
}

