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
};

struct MapData {
    int width, height;
    QList<Obstacle> obstacles;
    QPoint start, finish;
};

class Xml
{
    QString in_file_path;
    QString out_file_path;
public:
    Xml();
    MapData ReadFile(const QString&);
    void WriteFile(const QString&, const MapData&, const QList<QPoint>&, double, double);
};

#endif // XML_H
