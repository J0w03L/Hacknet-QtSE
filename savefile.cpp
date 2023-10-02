#include "savefile.h"

SaveFile::SaveFile()
{

}

int SaveFile::open(QString path)
{
    knownComputerIndexes.clear();
    computers.clear();
    computerCount = 0;

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

    // TODO: Validate that the XML file is really a Hacknet save file and not some random XML.

    qDebug() << "Loaded and parsed file to XML at path" << path << "successfully!";

    // Get known computers.
    QStringList knownIndexes = QString(this->doc.child("HacknetSave").child("NetworkMap").child("visible").text().get()).split(" ");
    for (QString index : knownIndexes)
    {
        if (index.compare("") != 0)
        {
            qDebug() << "Found known computer index:" << index;

            bool success;
            int indexNum = index.toInt(&success);

            if (!success)
            {
                qDebug() << "Known computer index of" << index << "is invalid; skipping!";
                continue;
            }

            this->knownComputerIndexes.append(indexNum);
        }
    }

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
        pugi::xml_node locationNode = computer.child("location");
        pugi::xml_node securityNode = computer.child("security");
        pugi::xml_node adminNode    = computer.child("admin");
        pugi::xml_node linksNode    = computer.child("links");
        pugi::xml_node firewallNode = computer.child("firewall");
        pugi::xml_node portsNode    = computer.child("portsOpen");
        pugi::xml_node usersNode    = computer.child("users");

        Node::Computer item;

        item.index = count;
        item.name = computer.attribute("name").as_string();
        item.ip = computer.attribute("ip").as_string();
        item.type = static_cast<Node::Computer::Type>(computer.attribute("type").as_int());
        item.spec = computer.attribute("spec").as_string();
        item.id = computer.attribute("id").as_string();

        item.known = this->knownComputerIndexes.contains(count);

        qDebug() << "Computer" << count << "is" << (item.known ? "known" : "not known") << "(" << item.known << ")";

        item.location.x = locationNode.attribute("x").as_double();
        item.location.y = locationNode.attribute("y").as_double();

        item.security.level = securityNode.attribute("level").as_int();
        item.security.traceTime = securityNode.attribute("traceTime").as_int();
        item.security.proxyTime = securityNode.attribute("proxyTime").as_int();
        item.security.portsToCrack = securityNode.attribute("portsToCrack").as_int();
        item.security.adminIP = securityNode.attribute("adminIP").as_string();

        item.admin.type = adminNode.attribute("type").as_string();
        item.admin.resetPass = adminNode.attribute("resetPass").as_bool();
        item.admin.isSuper = adminNode.attribute("isSuper").as_bool();

        qDebug() << "linkIndexes:" << linksNode.text().get();

        QStringList linkIndexes = QString(linksNode.text().get()).split(" ");
        for (QString index : linkIndexes)
        {
            if (index.compare("") != 0)
            {
                qDebug() << "Found link index:" << index;

                bool success;
                int indexNum = index.toInt(&success);

                if (!success)
                {
                    qDebug() << "Link index of" << index << "is invalid; skipping!";
                    continue;
                }

                item.links.append(indexNum);
            }
        }

        item.firewall.complexity = firewallNode.attribute("complexity").as_int();
        item.firewall.solution = firewallNode.attribute("solution").as_string();
        item.firewall.additionalDelay = firewallNode.attribute("additionalDelay").as_int();

        qDebug() << "portsOpen:" << portsNode.text().get();

        QStringList ports = QString(portsNode.text().get()).split(" ");
        for (QString port : ports)
        {
            if (port.compare("") != 0)
            {
                qDebug() << "Found port:" << port;

                bool success;
                int portNum = port.toInt(&success);

                if (!success)
                {
                    qDebug() << "Port" << port << "is invalid; skipping!";
                    continue;
                }

                item.portsOpen.append(static_cast<Node::Computer::Port>(portNum));
            }
        }

        for (pugi::xml_node user = usersNode.child("user"); user; user = user.next_sibling("user"))
        {
            Node::User userItem;

            userItem.name  = user.attribute("name").as_string();
            userItem.pass  = user.attribute("pass").as_string();
            userItem.type  = user.attribute("type").as_int();
            userItem.known = user.attribute("known").as_bool();

            item.users.append(userItem);
        }

        this->computers.append(item);

        qDebug() << "Computer" << count << ":" << item.name << ":" << item.ip;
        count++;
    }

    qDebug() << "Counted" << count << "computer nodes.";
    this->computerCount = count;

    return count;
}
