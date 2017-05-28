#include <QMap>
#include <QVector>
#include <QString>
#include <QDateTime>
#include <Aclapi.h>
#include <Lm.h>
#include <windows.h>
#include "petri.h"

struct entity {
    bool valid;
    char flags;
    QVector<bool> grant;
    QVector<bool> deny;
};

#ifndef FILERIGHT_H
#define FILERIGHT_H

class file {
private:
    QString path;
    QMap<QString, entity> rights;
    static void QDTtoSYST(QDateTime qdt, SYSTEMTIME &syst);
    static void SYSTtoQDT(SYSTEMTIME syst, QDateTime &qdt);
    static unsigned char parseAce(PVOID ace, QMap<QString, entity> &rights);
    static unsigned char psidToName(PSID psid, QString &name);
    static unsigned char nameToPsid(PSID &psid, QString name);
    static void parseMask(DWORD mask, QMap<QString, entity> &rights, QString key, bool access);
    static void parseFlags(DWORD &mask, QVector<bool> flags);
    static unsigned char addOther(QMap<QString, entity> &rights);
    static unsigned char getUsers(QVector<QString> &users);
    static unsigned char getGroups(QVector<QString> &groups);
    static unsigned char getAces(QVector<PVOID> &aces, QString name, entity parameters, bool access);
    static bool checkFlags(QVector<bool> flags);
public:
    file();
    file(QString currentPath);
    ~file();
    void setPath(QString currentPath);
    QString getPath();
    void getRights(QMap<QString, entity> &currentRights);
    void setRights(QMap<QString, entity> &currentRights);
    unsigned char SetFileRights();
    unsigned char GetFileRights();
    unsigned char SetAtributes(QVector<bool> atrs);
    unsigned char GetAtributes(QVector<bool> &atrs);
    unsigned char GetTime(QDateTime &create, QDateTime &access, QDateTime &write);
    unsigned char SetTime(QDateTime create, QDateTime access, QDateTime write);
    static unsigned char setFileRights(QString path, QMap<QString, entity> &rights);
    static unsigned char getFileRights(QString path, QMap<QString, entity> &rights);
    static unsigned char setAtributes(QString path, QVector<bool> atrs);
    static unsigned char getAtributes(QString path, QVector<bool> &atrs);
    static unsigned char getFileTime(QString path, QDateTime &create, QDateTime &access, QDateTime &write);
    static unsigned char setFileTime(QString path, QDateTime create, QDateTime access, QDateTime write);
};

#endif // FILERIGHT_H
