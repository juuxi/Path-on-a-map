#ifndef XML_H
#define XML_H

#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QPoint>
#include <QVector>

struct Obstacle { //описание "препятствия"
    int impassability;
    QVector<QPoint> points;
    bool operator==(const Obstacle& other) const {
        return points == other.points;
    }
};

struct MapData { //описание "данных карты", именно эта структура передается через "in" файл
    int width, height;
    QList<Obstacle> obstacles;
    QPoint start, finish;
    int left_map_margin; //используется для определения левой границы карты
};

struct PathData { //описание "данных пути", используется для вывода таблички с результатом после нажатия кнопки "Выполнить" и восстановления картинки
    int distance;
    int time;
    MapData mpdt;
};

class Xml
{
public:
    Xml();
    MapData ReadInFile(const QString&); //запись во входной файл
    PathData ReadOutFile(const QString&); //запись в выходной файл
    void WriteOutFile(const QString&, const MapData&, const QList<QPoint>&, double, double); //чтение выходного файла
    void WriteInFile(const QString&, const MapData&); //чтение входного файла
};

#endif // XML_H
