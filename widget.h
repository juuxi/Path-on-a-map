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
#include <QMessageBox>
#include <QMouseEvent>

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

    Map map;
    Xml xml;
    MapData mpdt;
    QVector<QPoint> new_poly_points;
    bool is_adding_obstacle;
    bool is_deleting_obstacle;
    bool is_executing;

public:
    Widget(QWidget *parent = nullptr);
    void Process();
    void paintEvent(QPaintEvent*);
    ~Widget();
protected:
    void resizeEvent(QResizeEvent*) override;
private:
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void SetupUI();
    void RepositionButtons();
private slots:
    void AddObstacleClicked();
    void DeleteObstacleClicked();
    void StartClicked();
    void FinishClicked();
    void ExecuteClicked();
};
#endif // WIDGET_H
