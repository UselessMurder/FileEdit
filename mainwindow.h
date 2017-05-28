#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QDateTimeEdit>
#include <QGridLayout>
#include <QStatusBar>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QSpacerItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QComboBox>
#include "container.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showTrial();
private:
    bool fl;
    unsigned char status;
    file currentFile;
    QGridLayout *grid;
    QLabel *lb1;
    QDateTimeEdit *dte1;
    QLabel *lb2;
    QDateTimeEdit *dte2;
    QLabel *lb3;
    QDateTimeEdit *dte3;
    QVector<QCheckBox*> atrList;
    QVector<QCheckBox*> rightList;
    QListWidget *nameList;
    QPushButton *alterName;
    QComboBox *accessType;
    void initMenuBar();
    void initGrid();
    void setGridEnabled(bool enable);
    unsigned char openFile();
    unsigned char saveFile();
private slots:
    void OpenFile();
    void SaveFile();
    void Quit();
    void About();
    void Changed();
    void RightChanged(int state);
    void ChangeName(QListWidgetItem* item);
    void AccessChange(int index);
    void AlterName();
};

#endif // MAINWINDOW_H
