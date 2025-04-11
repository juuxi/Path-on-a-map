#ifndef XML_H
#define XML_H

#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QPoint>

struct Obstacle {
    int impassability;
    QList<QPoint> points;
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
    void WriteFile();
};

#endif // XML_H
