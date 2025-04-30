#ifndef XML_H
#define XML_H

#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QPoint>
#include <QVector>

struct Obstacle {
    int impassability;
    QVector<QPoint> points;
    bool operator==(const Obstacle& other) const {
        return points == other.points;
    }
};

struct MapData {
    int width, height;
    QList<Obstacle> obstacles;
    QPoint start, finish;
    int left_map_margin;
};

struct PathData {
    int distance;
    int time;
};

class Xml
{
    QString in_file_path;
    QString out_file_path;
public:
    Xml();
    MapData ReadInFile(const QString&);
    PathData ReadOutFile(const QString&);
    void WriteOutFile(const QString&, const MapData&, const QList<QPoint>&, double, double);
    void WriteInFile(const QString&, const MapData&);
};

#endif // XML_H
