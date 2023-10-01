#include "savefile.h"

SaveFile::SaveFile()
{

}

int SaveFile::open(QString path)
{
    // TODO: Clean up if we've already opened a save file before.

    // First we check if the file actually exists.
    if (!QFile::exists(path))
    {
        qDebug() << "Could not find a file at path" << path << "!";
        return -1;
    }

    // It exists, so let's try to load it.
    pugi::xml_parse_result result = this->doc.load_file(path.toStdString().c_str());
    if (!result)
    {
        qDebug() << "Failed to load and parse file at path" << path << "!";
        qDebug() << "attr:" << this->doc.child("node").attribute("attr").value();
        qDebug() << "error desc:  " << result.description();
        qDebug() << "error offset:" << result.offset;
        return -2;
    }

    qDebug() << "Loaded and parsed file to XML at path" << path << "successfully!";

    // Index the computer nodes.
    qDebug() << "indexComputerNodes returned" << this->indexComputerNodes() << "computers.";

    return 1;
}

size_t SaveFile::indexComputerNodes()
{
    size_t count = 0;
    computers.clear();

    for (pugi::xml_node computer = this->doc.child("HacknetSave").child("NetworkMap").child("network").child("computer"); computer; computer = computer.next_sibling("computer"))
    {
        QString name = computer.attribute("name").as_string();
        QString ip = computer.attribute("ip").as_string();

        this->computers.append(QPair(ip, name));

        qDebug() << "Computer" << count << ":" << name << ":" << ip;
        count++;
    }

    qDebug() << "Counted" << count << "computer nodes.";
    this->computerCount = count;

    return count;
}
