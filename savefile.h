#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <pugixml.hpp>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QList>
#include <QPair>
#include <QDebug>
#include "nodes.h"

class SaveFile
{
public:
    SaveFile();
    int open(QString path);

    //QList<QPair<QString, QString>> computers;
    QList<Node::Computer> computers;
    QList<int> knownComputerIndexes;

private:
    size_t indexComputerNodes();

    pugi::xml_document doc;
    size_t computerCount = 0;
};

#endif // SAVEFILE_H
