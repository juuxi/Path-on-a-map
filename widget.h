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
#include <QInputDialog>

class Widget : public QWidget
{
    Q_OBJECT
    QPushButton *add_obstacle_btn;
    QPushButton *delete_obstacle_btn;
    QPushButton *start_btn;
    QPushButton *finish_btn; //кнопка "установить точку финиша"
    QPushButton *execute_btn;

    const int left_margin = 20;
    const int top_margin = 10;
    const int bottom_margin = 10;

    QLabel* start_point; //метка "старт"
    QLabel* finish_point; //метка "старт"
    QLabel* debug;
    Map map;
    Xml xml;
    MapData mpdt;
    QVector<QPoint> new_poly_points;
    bool is_adding_obstacle;
    bool is_deleting_obstacle;

public:
    Widget(QWidget *parent = nullptr);
    void Process();
    void paintEvent(QPaintEvent*);
    ~Widget();
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void setupUI();
    void reposition_buttons();
private slots:
    void add_obstacle_clicked();
    void delete_obstacle_clicked();
    void start_clicked();
    void finish_clicked();
    void execute_clicked();
};
#endif // WIDGET_H
