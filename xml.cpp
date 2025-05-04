#include "xml.h"

Xml::Xml() {

}

MapData Xml::ReadInFile(const QString& file_path) {
    MapData map_data;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << file_path;
        return map_data;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "Size") {
                map_data.width = xml.attributes().value("width").toInt();
                map_data.height = xml.attributes().value("height").toInt();
            } else if (xml.name() == "Margins") {
                map_data.left_map_margin = xml.attributes().value("left_map_margin").toInt();
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
                map_data.obstacles.append(obstacle);
            } else if (xml.name() == "Start") {
                map_data.start.setX(xml.attributes().value("x").toInt());
                map_data.start.setY(xml.attributes().value("y").toInt());
            } else if (xml.name() == "Finish") {
                map_data.finish.setX(xml.attributes().value("x").toInt());
                map_data.finish.setY(xml.attributes().value("y").toInt());
            }
        }
    }

    if (xml.hasError()) {
        qWarning() << "XML error:" << xml.errorString();
    }

    file.close();
    return map_data;
}

PathData Xml::ReadOutFile(const QString& file_path) {
    PathData path_data;
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << file_path;
        return path_data;
    }

    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == "Distance") {
                path_data.distance = xml.readElementText().toInt();
            } else if (xml.name() == "Time") {
                path_data.time = xml.readElementText().toInt();
            } else if (xml.name() == "Size") {
                path_data.mpdt.width = xml.attributes().value("width").toInt();
                path_data.mpdt.height = xml.attributes().value("height").toInt();
            } else if (xml.name() == "Margins") {
                path_data.mpdt.left_map_margin = xml.attributes().value("left_map_margin").toInt();
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
                path_data.mpdt.obstacles.append(obstacle);
            } else if (xml.name() == "Start") {
                path_data.mpdt.start.setX(xml.attributes().value("x").toInt());
                path_data.mpdt.start.setY(xml.attributes().value("y").toInt());
            } else if (xml.name() == "Finish") {
                path_data.mpdt.finish.setX(xml.attributes().value("x").toInt());
                path_data.mpdt.finish.setY(xml.attributes().value("y").toInt());
            }
        }
    }

    if (xml.hasError()) {
        qWarning() << "XML error:" << xml.errorString();
    }

    file.close();
    return path_data;
}

void Xml::WriteOutFile(const QString& file_path, const MapData& input_data, const QList<QPoint>& path, double distance, double time) {
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << file_path;
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Route");

    // Input section
    xml.writeStartElement("Input");
    xml.writeStartElement("Size");
    xml.writeAttribute("width", QString::number(input_data.width));
    xml.writeAttribute("height", QString::number(input_data.height));
    xml.writeEndElement(); // Size

    xml.writeStartElement("Margins");
    xml.writeAttribute("left_map_margin", QString::number(input_data.left_map_margin));
    xml.writeEndElement();

    xml.writeStartElement("Obstacles");
    for (const auto& obstacle : input_data.obstacles) {
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
    xml.writeAttribute("x", QString::number(input_data.start.x()));
    xml.writeAttribute("y", QString::number(input_data.start.y()));
    xml.writeEndElement(); // Start

    xml.writeStartElement("Finish");
    xml.writeAttribute("x", QString::number(input_data.finish.x()));
    xml.writeAttribute("y", QString::number(input_data.finish.y()));
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

void Xml::WriteInFile(const QString& file_path, const MapData& input_data){
    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << file_path;
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("Map");

    // Input section
    xml.writeStartElement("Size");
    xml.writeAttribute("width", QString::number(input_data.width));
    xml.writeAttribute("height", QString::number(input_data.height));
    xml.writeEndElement(); // Size

    xml.writeStartElement("Margins");
    xml.writeAttribute("left_map_margin", QString::number(input_data.left_map_margin));
    xml.writeEndElement();

    xml.writeStartElement("Obstacles");
    for (const auto& obstacle : input_data.obstacles) {
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
    xml.writeAttribute("x", QString::number(input_data.start.x()));
    xml.writeAttribute("y", QString::number(input_data.start.y()));
    xml.writeEndElement(); // Start

    xml.writeStartElement("Finish");
    xml.writeAttribute("x", QString::number(input_data.finish.x()));
    xml.writeAttribute("y", QString::number(input_data.finish.y()));
    xml.writeEndElement(); // Finish

    xml.writeEndElement(); // Input

    xml.writeEndElement(); // Route
    xml.writeEndDocument();

    file.close();
}
