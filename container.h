#ifndef CONTAINER_H
#define CONTAINER_H

#include <QString>
#include <QDateTime>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include "file.h"
#include "trash.h"

union rlb
{
    long l;
    char b[4];
};

union rllb
{
    long long ll;
    char b[8];
};

union rlll
{
    long long ll;
    long l[2];
};

union rshb
{
    short int sh;
    char b[2];
};

class container {
private:
    static void alterValue(unsigned long long &value, unsigned __int64 key);
    static void alterByte(char &byte, long key, short int subkey, bool flag);
    static void hideContainer(QString path);
    static void unhideContainer(QString path);
    static void check(bool &value);
    static unsigned char create();
public:
    static unsigned char getKey(unsigned __int64 &key);
    static unsigned char setKey(unsigned __int64 &key);
};

#endif // CONTAINER_H
