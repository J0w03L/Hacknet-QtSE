#ifndef NODES_H
#define NODES_H

#include <QString>
#include <QList>

namespace Node
{
    typedef struct Location
    {
        double x;
        double y;
    } Location;

    typedef struct Security
    {
        int level;
        int traceTime;
        int proxyTime;
        int portsToCrack;
        QString adminIP;
    } Security;

    typedef struct Admin
    {
        QString type;
        bool resetPass;
        bool isSuper;
    } Admin;

    typedef struct Firewall
    {
        int complexity;
        QString solution;
        int additionalDelay;
    } Firewall;

    typedef struct User
    {
        QString name;
        QString pass;
        int type;
        bool known;
    } User;

    typedef struct Computer
    {
        QString index;
        QString name;
        QString ip;

        typedef enum Type : int
        {
            CORPORATE   = 1,
            HOME        = 2,
            SERVER      = 3,
            EMPTY       = 4,
            EOS         = 5
        } Type;
        Type type;

        QString spec;
        QString id;

        Location location;
        bool known;
        Security security;
        Admin admin;
        QList<int> links;
        Firewall firewall;

        typedef enum Port : int
        {
            SSH = 22,
            FTP = 21,
            SMTP = 25,
            HTTP = 80,
            BLIZZARD = 3724,
            SQL = 1433,
            MEDICAL = 104,
            EOSCON = 3659
        } Port;
        QList<Port> portsOpen;

        QList<User> users;
    } Computer;
}

#endif // NODES_H
