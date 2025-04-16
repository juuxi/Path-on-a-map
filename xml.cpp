#include "xml.h"

Xml::Xml() {

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

void Xml::WriteOutFile(const QString& filePath, const MapData& inputData, const QList<QPoint>& path, double distance, double time) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Route");

    // Input section
    xml.writeStartElement("Input");
    xml.writeStartElement("Size");
    xml.writeAttribute("width", QString::number(inputData.width));
    xml.writeAttribute("height", QString::number(inputData.height));
    xml.writeEndElement(); // Size

    xml.writeStartElement("Obstacles");
    for (const auto& obstacle : inputData.obstacles) {
        xml.writeStartElement("Obstacle");
        xml.writeAttribute("impassability", QString::number(obstacle.impassability));

        xml.writeStartElement("Points");
        xml.writeAttribute("count", QString::number(obstacle.points.size()));
        for (const auto& point : obstacle.points) {
            xml.writeStartElement("Point");
            xml.writeAttribute("x", QString::number(point.x()));
            xml.writeAttribute("y", QString::number(point.y()));
            xml.writeEndElement(); // Point
        }
        xml.writeEndElement(); // Points

        xml.writeEndElement(); // Obstacle
    }
    xml.writeEndElement(); // Obstacles

    xml.writeStartElement("Start");
    xml.writeAttribute("x", QString::number(inputData.start.x()));
    xml.writeAttribute("y", QString::number(inputData.start.y()));
    xml.writeEndElement(); // Start

    xml.writeStartElement("Finish");
    xml.writeAttribute("x", QString::number(inputData.finish.x()));
    xml.writeAttribute("y", QString::number(inputData.finish.y()));
    xml.writeEndElement(); // Finish

    xml.writeEndElement(); // Input

    // Path section
    xml.writeStartElement("Path");
    xml.writeStartElement("Points");
    xml.writeAttribute("count", QString::number(path.size()));
    for (const auto& point : path) {
        xml.writeStartElement("Point");
        xml.writeAttribute("x", QString::number(point.x()));
        xml.writeAttribute("y", QString::number(point.y()));
        xml.writeEndElement(); // Point
    }
    xml.writeEndElement(); // Points
    xml.writeEndElement(); // Path

    // Statistics section
    xml.writeStartElement("Statistics");
    xml.writeTextElement("Distance", QString::number(distance));
    xml.writeTextElement("Time", QString::number(time));
    xml.writeEndElement(); // Statistics

    xml.writeEndElement(); // Route
    xml.writeEndDocument();

    file.close();
}

void Xml::WriteInFile(const QString& filePath, const MapData& inputData){
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Map");

    // Input section
    xml.writeStartElement("Size");
    xml.writeAttribute("width", QString::number(inputData.width));
    xml.writeAttribute("height", QString::number(inputData.height));
    xml.writeEndElement(); // Size

    xml.writeStartElement("Obstacles");
    for (const auto& obstacle : inputData.obstacles) {
        xml.writeStartElement("Obstacle");
        xml.writeAttribute("impassability", QString::number(obstacle.impassability));

        xml.writeStartElement("Points");
        xml.writeAttribute("count", QString::number(obstacle.points.size()));
        for (const auto& point : obstacle.points) {
            xml.writeStartElement("Point");
            xml.writeAttribute("x", QString::number(point.x()));
            xml.writeAttribute("y", QString::number(point.y()));
            xml.writeEndElement(); // Point
        }
        xml.writeEndElement(); // Points

        xml.writeEndElement(); // Obstacle
    }
    xml.writeEndElement(); // Obstacles

    xml.writeStartElement("Start");
    xml.writeAttribute("x", QString::number(inputData.start.x()));
    xml.writeAttribute("y", QString::number(inputData.start.y()));
    xml.writeEndElement(); // Start

    xml.writeStartElement("Finish");
    xml.writeAttribute("x", QString::number(inputData.finish.x()));
    xml.writeAttribute("y", QString::number(inputData.finish.y()));
    xml.writeEndElement(); // Finish

    xml.writeEndElement(); // Input

    xml.writeEndElement(); // Route
    xml.writeEndDocument();

    file.close();
}
