#include "xml.h"

Xml::Xml()
{

}

MapData Xml::ReadFile(const QString& filePath) {
    MapData mapData;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return mapData;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "Size") {
                mapData.width = xml.attributes().value("width").toInt();
                mapData.height = xml.attributes().value("height").toInt();
            } else if (xml.name() == "Obstacle") {
                Obstacle obstacle;
                obstacle.impassability = xml.attributes().value("impassability").toInt();
                while (!(xml.name() == "Obstacle" && xml.isEndElement())) {
                    xml.readNext();
                    if (xml.name() == "Point" && xml.isStartElement()) {
                        QPoint point;
                        point.setX(xml.attributes().value("x").toInt());
                        point.setY(xml.attributes().value("y").toInt());
                        obstacle.points.append(point);
                    }
                }
                mapData.obstacles.append(obstacle);
            } else if (xml.name() == "Start") {
                mapData.start.setX(xml.attributes().value("x").toInt());
                mapData.start.setY(xml.attributes().value("y").toInt());
            } else if (xml.name() == "Finish") {
                mapData.finish.setX(xml.attributes().value("x").toInt());
                mapData.finish.setY(xml.attributes().value("y").toInt());
            }
        }
    }

    if (xml.hasError()) {
        qWarning() << "XML error:" << xml.errorString();
    }

    file.close();
    return mapData;
}
