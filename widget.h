#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <map.h>

class Widget : public QWidget
{
    Q_OBJECT
    QPushButton* implement; //кнопка "выполнить" (найти путь и отобрзить его)
    QPushButton* add; //кнопка "добавить препятствие"
    QPushButton* remove; //кнопка "удалить препятствие"
    QPushButton* start; //кнопка "установить точку старта"
    QPushButton* finish; //кнопка "установить точку финиша"
    Map map;

public:
    Widget(QWidget *parent = nullptr);
    void Process();
    ~Widget();
};
#endif // WIDGET_H
