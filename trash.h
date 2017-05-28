#ifndef TRASH_H
#define TRASH_H
#include "asm_instruction.h"

class trash {
private:
    static unsigned short int gen();
public:
    static void trashFunc(unsigned long long &trash1, unsigned long long &trash2);
    static unsigned char trashCheck(void (*check)(bool&), unsigned char (*write)());
};

#endif // TRASH_H
