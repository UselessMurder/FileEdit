#include "mainwindow.h"
#include <QApplication>


void NextMain(unsigned __int64 key, int argc, char *argv[]) {
    petri::Check();
    static unsigned __int64 currentKey = key;
    static unsigned long long trashKey = 0;
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_1);
    push_32(&trash::trashFunc);
    ret;
    next_1:
    if(container::setKey(key)) {
        push_32(&trashKey);
        push_32(&currentKey);
        push_32(&&next_2);
        push_32(&trash::trashFunc);
        ret;
        next_2:
        petri::Exit();
        return;
    }
    push_32(&trashKey);
    push_32(&currentKey);
    push_32(&&next_3);
    push_32(&trash::trashFunc);
    ret;
    next_3:
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(Quit()));
    w.show();
    a.exec();
}

void ExitMain(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.showTrial();
    return a.exit();
}

int main(int argc, char *argv[]) {
    static unsigned __int64 key = 0;
    static unsigned long long trashKey = 0;
    push_32(&trashKey);
    push_32(&key);
    push_32(&&next);
    push_32(&trash::trashFunc);
    ret;
    next:
    if(!container::getKey(key))
        petri::Main(key, &NextMain, &ExitMain, argc, argv);
    return 0;
}

