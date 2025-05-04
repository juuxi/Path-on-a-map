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
    QPushButton *finish_btn;
    QPushButton *execute_btn;

    const int left_margin = 20;
    const int top_margin = 10;
    const int bottom_margin = 10;

    Map map;
    Xml xml;
    MapData mpdt;
    QVector<QPoint> new_poly_points;

    bool is_adding_obstacle; //флаг нажатия кнопки "добавить объект"
    bool is_deleting_obstacle; //флаг нажатия кнопки "удалить объект"
    bool is_executing; //флаг нажатия кнопки "выполнить"
    bool is_changing_start; //флаг нажатия кнопки "старт"
    bool is_changing_finish; //флаг нажатия кнопки "финиш"

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent*); //событие "отрисовки" (автоматически вызывается при вызове update())
    ~Widget();
protected:
    void resizeEvent(QResizeEvent*) override; //событие измения размеров экрана
private:
    void mousePressEvent(QMouseEvent*); //событие одинарного щелчка мыши
    void mouseDoubleClickEvent(QMouseEvent*); //событие двойного щелчка мыши
    void SetupUI(); //функция, в которой создаются все UI элементы
    void RepositionButtons(); //расставление интервалов между кнопками
private slots:
    void AddObstacleClicked(); //слоты, обрабатывающие нажатия на соответствующие кнопки
    void DeleteObstacleClicked();
    void StartClicked();
    void FinishClicked();
    void ExecuteClicked();
};
#endif // WIDGET_H
