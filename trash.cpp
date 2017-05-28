#include <ctime>
#include <random>
#include <stdio.h>
#include <QFile>
#include <QString>
#include "trash.h"


unsigned short int trash::gen() {
    int t = 0;
    push_32(&&rand1);
    ret;
    rand0:
    get_eax(t);
    push_32(t);
    push_32(&&rand2);
    push_32(&srand);
    ret;
    rand1:
    push_32(NULL);
    push_32(&&rand0);
    jmp(&time);
    rand2:
    unsigned short int val[2] = {0,};
    unsigned short int value = 0;
    unsigned int i = 0;
    jmp(&&randSwitch);
    rand3:
    get_ax(value);
    val[i] = value;
    i++;
    ret;
    randSwitch:
    switch (i) {
       case 0: {
            push_32(&&case_0);
            push_32(&&rand3);
            jmp(&rand);
            case_0:
            break;
        }
       case 1: {
            push_32(&&case_1);
            push_32(&&rand3);
            push_32(&rand);
            ret;
            case_1:
            break;
        }
       case 2: {
            jmp(&&rand_end);
        }
    }
    jmp(&&randSwitch);
    rand_end:
    return val[0] % (val[1] + 1);
}

void trash::trashFunc(unsigned long long &trash1, unsigned long long &trash2) {
    trash1--;
    push_32(&&init);
    ret;
    met0:
    jmp(&&end);
    end:
    trash1++;
    jmp(&&exit);
    init:
    unsigned short int i = 0;
    push_32(&&next);
    push_32(&gen);
    ret;
    next:
    get_ax(i);
    int mas[32] = {i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i,};
    jmp(&&begin);
    met1:
    trash1 = trash1;
    i--;
    push_32(&&met3);
    ret;
    met2:
    i--;
    trash2--;
    jmp(&&begin);
    met3:
    trash1 = trash1;
    trash2++;
    i--;
    push_32(&&met5);
    ret;
    met4:
    i--;
    jmp(&&met2);
    met5:
    trash2 += trash1 - trash2;
    i--;
    push_32(&&met6);
    ret;
    met6:
    i--;
    jmp(&&met4);
    begin:
    mas[i%32] = i;
    if (i > 0) {
        switch(i%6) {
           case 0:
            jmp(&&met1);
           case 1:
            push_32(&&met2);
            ret;
           case 2:
            push_32(&&met3);
            ret;
           case 3:
            jmp(&&met4);
           case 4:
            push_32(&&met5);
            ret;
           case 5:
            jmp(&&met6);
           default:
            push_32(&&met0);
            ret;
        }
    } else {
        jmp(&&met0);
    }
    exit:
    return;
}

unsigned char trash::trashCheck(void (*check)(bool &), unsigned char (*write)()) {
    unsigned char code = 0;
    bool flag = false;
    bool ending = false;
    push_32(&&init);
    ret;
    met0:
    jmp(&&end);
    end:
    jmp(&&exit);
    init:
    unsigned short int i = 0;
    push_32(&&next);
    push_32(&gen);
    ret;
    next:
    get_ax(i);
    int mas[32] = {i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i,};
    jmp(&&begin);
    met1:
    i--;
    push_32(&&met3);
    ret;
    met2:
    i--;
    jmp(&&begin);
    met3:
    i--;
    push_32(&&met5);
    ret;
    met4:
    i--;
    jmp(&&met2);
    met5:
    i--;
    push_32(&&met6);
    ret;
    met6:
    i--;
    jmp(&&met4);
    begin:
    mas[i%32] = i;
    check(flag);
    if (i > 0) {
        switch(i%6) {
           case 0:
            jmp(&&met1);
           case 1:
            push_32(&&met2);
            ret;
           case 2:
            push_32(&&met3);
            ret;
           case 3:
            jmp(&&met4);
           case 4:
            push_32(&&met5);
            ret;
           case 5:
            jmp(&&met6);
           default:
            push_32(&&met0);
            ret;
        }
    } else {
        if (!ending) {
            if (!flag)
                code = write();
            ending = true;
            jmp(&&init);
        }
        jmp(&&met0);
    }
    exit:
    return code;
}



