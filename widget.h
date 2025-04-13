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
#include <QDir>

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

public:
    Widget(QWidget *parent = nullptr);
    void Process();
    ~Widget();
};
#endif // WIDGET_H
