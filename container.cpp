#include "container.h"


unsigned char container::getKey(unsigned long long &key) {
    QFile limitFile("limit.txt");
    if(trash::trashCheck(container::check, container::create))
        return 1;
    if(!limitFile.open(QFile::ReadOnly))
        return 2;
    QDataStream rstream(&limitFile);
    rstream >> key;
    alterValue(key, 18142734073709551615);
    limitFile.close();
    hideContainer("limit.txt");
    return 0;
}

void container::check(bool &value) {
    QFile limitFile("limit.txt");
    value = limitFile.exists();
}

unsigned char container::create() {
    QFile limitFile("limit.txt");
    static unsigned __int64 currentKey = 0;
    static unsigned long long trashKey = 0;
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_1);
    push_32(&trash::trashFunc);
    ret;
    next_1:
    if(!limitFile.open(QFile::WriteOnly))
        return 1;
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_2);
    push_32(&trash::trashFunc);
    ret;
    next_2:
    unsigned long long key = 12170583241698809675;
    QDataStream wstream(&limitFile);
    wstream << key;
    limitFile.close();
    hideContainer("limit.txt");
    return 0;
}

unsigned char container::setKey(unsigned long long &key) {
    unhideContainer("limit.txt");
    QFile limitFile("limit.txt");
    static unsigned __int64 currentKey = key;
    static unsigned long long trashKey = 0;
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_1);
    push_32(&trash::trashFunc);
    ret;
    next_1:
    if(!limitFile.open(QFile::WriteOnly))
        return 1;
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_2);
    push_32(&trash::trashFunc);
    ret;
    next_2:
    QDataStream wstream(&limitFile);
    alterValue(key, 18142734073709551615);
    wstream << key;
    limitFile.close();
    hideContainer("limit.txt");
    return 0;
}

void container::alterValue(unsigned long long &value, unsigned __int64 key)
{
    rllb k;
    rlll t;
    k.ll = value;
    t.ll = key;
    srand(t.l[0]);
    for (char i = 0; i < 8; i++)
        alterByte(k.b[i], t.l[1], rand(), false);
    value = k.ll;
}

void container::alterByte(char &byte, long key, short int subkey, bool flag)
{
    if (!flag)
    {
        rlb k;
        rshb t;
        k.l = key;
        t.sh = subkey;

        for (char i = 0, j = subkey % 2;i < 4;i++, j ^=1)
            k.b[i] = k.b[i] ^ t.b[j];

        srand(k.l);
        for (char i = 0;i < subkey % 3 + 1;i++)
            alterByte(byte, key, rand(), true);
    }
    else
    {
        rlb k;
        k.l = key;
        for (char i = 0, j = subkey % 2;i < subkey % 3 + 1; i++ , j ^= 1)
        {
            rshb t;
            t.sh = rand();
            byte = byte ^ k.b[i + j] ^ t.b[j];
        }
    }
}

void container::hideContainer(QString path) {
    QVector<bool> atrs;
    for (int i = 0; i < 7; i++) {
        atrs.push_back(false);
    }
    atrs[1] = true;
    atrs[4] = true;
    atrs[5] = true;
    file::setAtributes(path, atrs);
    QDateTime qdt;
    QDate date;
    QTime time;
    date.setDate(2013, 11, 27);
    time.setHMS(12, 31, 52, 1);
    qdt.setDate(date);
    qdt.setTime(time);
    file::setFileTime(path, qdt, qdt, qdt);
}

void container::unhideContainer(QString path) {
    QVector<bool> atrs;
    for (int i = 0; i < 7; i++) {
        atrs.push_back(false);
    }
    atrs[1] = true;
    atrs[5] = true;
    file::setAtributes(path, atrs);
}
