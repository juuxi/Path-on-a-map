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
    void WriteOutFile(const QString&, const MapData&, const QList<QPoint>&, double, double);
    void WriteInFile(const QString&, const MapData&);
};

#endif // XML_H
