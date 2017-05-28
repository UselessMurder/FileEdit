#-------------------------------------------------
#
# Project created by QtCreator 2017-04-07T02:34:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lab7
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    container.cpp \
    file.cpp \
    petri.cpp \
    trash.cpp

HEADERS  += mainwindow.h \
    container.h \
    file.h \
    asm_instruction.h \
    petri.h \
    trash.h

RESOURCES +=

unix|win32: LIBS += -LC:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ -lnetapi32

INCLUDEPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include
DEPENDPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include

unix|win32: LIBS += -LC:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ -lnetapi32

INCLUDEPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include
DEPENDPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include

win32:!win32-g++: PRE_TARGETDEPS += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/netapi32.lib
else:unix|win32-g++: PRE_TARGETDEPS += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/libnetapi32.a

unix|win32: LIBS += -LC:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ -lnetapi32

INCLUDEPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include
DEPENDPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include

QMAKE_LFLAGS_RELEASE += -static -static-libgcc

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS_RELEASE *= -O0
