#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QEventLoop>
#include <QBitmap>
#include <map.h>
#include <QPainter>

class Widget : public QWidget
{
    Q_OBJECT
    QPushButton* implement; //кнопка "выполнить" (найти путь и отобрзить его)
    QPushButton* add; //кнопка "добавить препятствие"
    QPushButton* remove; //кнопка "удалить препятствие"
    QPushButton* start; //кнопка "установить точку старта"
    QPushButton* finish; //кнопка "установить точку финиша"
    QLabel* start_point; //метка "старт"
    QLabel* finish_point; //метка "старт"
    QLabel* debug;
    Map map;
    QVector<QPoint> new_poly_points;
    bool is_adding_obstacle;

public:
    Widget(QWidget *parent = nullptr);
    void Process();
    void paintEvent(QPaintEvent*);
    ~Widget();
private:
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
};
#endif // WIDGET_H
