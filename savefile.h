#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <pugixml.hpp>
#include <QString>
#include <QFile>
#include <QList>
#include <QPair>
#include <QDebug>

class SaveFile
{
public:
    SaveFile();
    int open(QString path);

    QList<QPair<QString, QString>> computers;

private:
    size_t indexComputerNodes();

    pugi::xml_document doc;
    size_t computerCount = 0;
};

#endif // SAVEFILE_H
