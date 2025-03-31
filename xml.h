#ifndef XML_H
#define XML_H

#include <QString>

class Xml
{
    QString in_file_path;
    QString out_file_path;
public:
    Xml();
    void ReadFile();
    void WriteFile();
};

#endif // XML_H
