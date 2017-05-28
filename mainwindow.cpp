#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->initMenuBar();
    this->initGrid();
    this->setGridEnabled(false);
    statusBar()->showMessage(tr("Файл не выбран!"));
    QWidget *base = new QWidget;
    base->setLayout(grid);
    setCentralWidget(base);
    fl = true;
    status = 0;
}

void MainWindow::showTrial() {
    QMessageBox::warning(this, "Ограничение", "Лимит запуска приложения исчерпан!");
}

void MainWindow::initMenuBar() {
    QMenu *file = menuBar()->addMenu("Файл");
    QMenu *help = menuBar()->addMenu("Помощь");

    QAction *openFile = new QAction(tr("Открыть"), this);
    QAction *saveFile = new QAction(tr("Сохранить"), this);
    QAction *quit = new QAction(tr("Завершить"), this);
    QAction *about = new QAction(tr("Обо мне"), this);

    openFile->setShortcut(QKeySequence::Open);
    saveFile->setShortcut(QKeySequence::Save);
    quit->setShortcut(QKeySequence::Close);
    about->setShortcut(QKeySequence::HelpContents);

    connect(openFile, SIGNAL(triggered()), this, SLOT(OpenFile()));
    connect(saveFile, SIGNAL(triggered()), this, SLOT(SaveFile()));
    connect(quit, SIGNAL(triggered()), this, SLOT(Quit()));
    connect(about, SIGNAL(triggered()), this, SLOT(About()));

    file->addAction(openFile);
    file->addAction(saveFile);
    file->addAction(quit);
    help->addAction(about);
}

unsigned char MainWindow::openFile() {
    unsigned char code = 0;
    QDateTime create, access, write;
    code = currentFile.GetTime(create, access, write);
    if (code) {
        return code;
    }
    dte1->setDateTime(create);
    dte2->setDateTime(access);
    dte3->setDateTime(write);
    QVector<bool> atrs;
    code = currentFile.GetAtributes(atrs);
    if (code) {
        return code;
    }
    for (int i = 0; i < atrs.size(); i++) {
        if (atrs[i]) {
            atrList[i]->setCheckState(Qt::Checked);
        } else {
            atrList[i]->setCheckState(Qt::Unchecked);
        }
    }
    code = currentFile.GetFileRights();
    if (code) {
        return code;
    }
    QMap<QString, entity> list;
    accessType->setEnabled(false);
    alterName->setEnabled(false);
    for (int i=0; i < rightList.size(); i++) {
        rightList[i]->setEnabled(false);
    }
    nameList->clear();
    currentFile.getRights(list);
    QMap<QString, entity>::const_iterator i = list.constBegin();
    while (i != list.constEnd()) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(i.key());
        if (i.value().valid) {
             item->setBackgroundColor(QColor(0, 255, 0, 127));
        } else {
             item->setBackgroundColor(QColor(255, 0, 0, 127));
        }
        nameList->insertItem(0,item);
        ++i;
    }
    return code;
}

unsigned char MainWindow::saveFile() {
    unsigned char code = 0;
    code = currentFile.SetTime(dte1->dateTime(), dte2->dateTime(), dte3->dateTime());
    if (code) {
        return code;
    }
    QVector<bool> atrs;
    for (int i = 0; i < atrList.size(); i++) {
        if (atrList[i]->checkState() == Qt::Checked) {
            atrs.push_back(true);
        } else {
            atrs.push_back(false);
        }
    }
    code = currentFile.SetAtributes(atrs);
    code = currentFile.SetFileRights();
    return code;
}

void MainWindow::Changed() {
    if (status == 2) {
        status = 1;
    }
}

void MainWindow::ChangeName(QListWidgetItem *item) {
    QString key = item->text();
    QMap<QString, entity> list;
    currentFile.getRights(list);
    alterName->setEnabled(true);
    fl = false;
    if (list[key].valid) {
        accessType->setEnabled(true);
        for (int i=0; i < rightList.size(); i++) {
            rightList[i]->setEnabled(true);
        }
        if (accessType->currentIndex() == 0) {
            for (int i = 0; i < rightList.size(); i++) {
                if (list[key].grant[i]) {
                   rightList[i]->setCheckState(Qt::Checked);
                } else {
                   rightList[i]->setCheckState(Qt::Unchecked);
                }
            }
        } else {
            for (int i = 0; i < rightList.size(); i++) {
                if (list[key].deny[i]) {
                   rightList[i]->setCheckState(Qt::Checked);
                } else {
                   rightList[i]->setCheckState(Qt::Unchecked);
                }
            }
        }
    } else {
        accessType->setEnabled(false);
        for (int i=0; i < rightList.size(); i++) {
            rightList[i]->setEnabled(false);
        }
    }
    fl = true;
}

void MainWindow::AccessChange(int index) {
    QString key = nameList->currentItem()->text();
    QMap<QString, entity> list;
    currentFile.getRights(list);
    fl = false;
    if (index == 0) {
        for (int i = 0; i < rightList.size(); i++) {
            if (list[key].grant[i]) {
               rightList[i]->setCheckState(Qt::Checked);
            } else {
               rightList[i]->setCheckState(Qt::Unchecked);
            }
        }
    } else {
        for (int i = 0; i < rightList.size(); i++) {
            if (list[key].deny[i]) {
               rightList[i]->setCheckState(Qt::Checked);
            } else {
               rightList[i]->setCheckState(Qt::Unchecked);
            }
        }
    }
    fl = true;
}

void MainWindow::AlterName() {
    QString key = nameList->currentItem()->text();
    QMap<QString, entity> list;
    currentFile.getRights(list);
    if(list[key].valid) {
        list[key].valid = false;
        nameList->currentItem()->setBackgroundColor(QColor(255, 0, 0, 127));
        accessType->setEnabled(false);
        for (int i=0; i < rightList.size(); i++) {
            rightList[i]->setEnabled(false);
            rightList[i]->setCheckState(Qt::Unchecked);
        }
    } else {
        list[key].valid = true;
        nameList->currentItem()->setBackgroundColor(QColor(0, 255, 0, 127));
        accessType->setEnabled(true);
        for (int i=0; i < rightList.size(); i++) {
            rightList[i]->setEnabled(true);
        }
        if (!accessType->currentIndex()) {
            for (int i = 0; i < rightList.size(); i++) {
                if (list[key].grant[i]) {
                   rightList[i]->setCheckState(Qt::Checked);
                } else {
                   rightList[i]->setCheckState(Qt::Unchecked);
                }
            }
        } else {
            for (int i = 0; i < rightList.size(); i++) {
                if (list[key].deny[i]) {
                   rightList[i]->setCheckState(Qt::Checked);
                } else {
                   rightList[i]->setCheckState(Qt::Unchecked);
                }
            }
        }
    }
    currentFile.setRights(list);
    emit(this->Changed());
}

void MainWindow::OpenFile() {
    petri::Check();
    unsigned char code = 0;
    switch (status) {
        case 1: {
           int reply = QMessageBox::question(this,"Внимание","Сохранить изменения в текущем файле?", QMessageBox::Yes, QMessageBox::No);
           if (reply == (int)QMessageBox::Yes) {
                emit(this->SaveFile());
           }
        }
        default: {
           QString path = QFileDialog::getOpenFileName(this, tr("Выберите файл"));
            if (path != "") {
                currentFile.setPath(path);
                code = this->openFile();
                if (!code) {
                    status = 2;
                    this->setGridEnabled(true);
                    statusBar()->showMessage(tr("Выбран файл: ") + path);
                } else {
                    status = 0;
                    this->setGridEnabled(false);
                    statusBar()->showMessage(tr("Ошибка при открытии файла: ") + currentFile.getPath());
                }
            } else {
                status = 0;
                this->setGridEnabled(false);
                statusBar()->showMessage(tr("Файл не выбран!"));
            }
        }
    }
    petri::Check();
}

void MainWindow::SaveFile() {
    petri::Check();
    unsigned char code = 0;
    switch (status) {
        case 1: {
            code = this->saveFile();
            if (!code) {
                status = 2;
                this->setGridEnabled(true);
                statusBar()->showMessage(tr("Изменения сохранены: ") + currentFile.getPath());
            } else {
                status = 0;
                this->setGridEnabled(false);
                statusBar()->showMessage(tr("Ошибка при сохранении изменений: ") + currentFile.getPath());
            }
            break;
        }
        default: {
            statusBar()->showMessage(tr("Невозможно сохранить изменения!"));
        }
    }
    petri::Check();
}

void MainWindow::Quit() {
    petri::Check();
    switch (status) {
        case 1: {
        int reply = QMessageBox::question(this,"Внимание","Сохранить изменения в текущем файле?", QMessageBox::Yes, QMessageBox::No);
        if (reply == (int)QMessageBox::Yes) {
             emit(this->SaveFile());
        }
        }
        default: {
            petri::Exit();
            this->close();
        }
    }
    petri::Check();
}

void MainWindow::RightChanged(int state) {
    int position = 0;
    if (fl) {
        QString key = nameList->currentItem()->text();
        QMap<QString, entity> list;
        currentFile.getRights(list);
        for (int i = 0; i < rightList.size(); i++) {
            if (rightList[i]->checkState() == Qt::Checked) {
                if(accessType->currentIndex()) {
                    if (list[key].deny[i] != true) {
                        position = i;
                        break;
                    }
                } else {
                    if (list[key].grant[i] != true) {
                        position = i;
                        break;
                    }
                }
            } else {
                if(accessType->currentIndex()) {
                    if (list[key].deny[i] != false) {
                        position = i;
                        break;
                    }
                } else {
                    if (list[key].grant[i] != false) {
                        position = i;
                        break;
                    }
                }
            }
        }
        fl = false;
        if (state) {
            if (accessType->currentIndex()) {
                if (position != 0) {
                    list[key].grant[position] = false;
                    list[key].deny[position] = true;
                    list[key].grant[0] = false;
                } else {
                    list[key].deny[0] = true;
                    list[key].grant[0] = false;
                    for(int i = 1; i < rightList.size(); i++) {
                        rightList[i]->setCheckState(Qt::Checked);
                        list[key].deny[i] = true;
                        list[key].grant[i] = false;
                    }
                }
            } else {
                if (position != 0) {
                    list[key].grant[position] = true;
                    list[key].deny[position] = false;
                    list[key].deny[0] = false;
                } else {
                    list[key].deny[0] = false;
                    list[key].grant[0] = true;
                    for(int i = 1; i < rightList.size(); i++) {
                        rightList[i]->setCheckState(Qt::Checked);
                        list[key].deny[i] = false;
                        list[key].grant[i] = true;
                    }
                }
            }
        } else {
            if (accessType->currentIndex()) {
                if (position != 0) {
                    list[key].deny[position] = false;
                    list[key].deny[0] = false;
                    rightList[0]->setCheckState(Qt::Unchecked);
                } else {
                    list[key].grant[0] = false;
                }
            } else {
                if (position != 0) {
                    list[key].grant[position] = false;
                    list[key].grant[0] = false;
                    rightList[0]->setCheckState(Qt::Unchecked);
                } else {
                    list[key].grant[0] = false;
                }
            }
        }
        fl = true;
        currentFile.setRights(list);
        emit(this->Changed());
    }
}

void MainWindow::About() {
    petri::Check();
    QMessageBox::information(this, "Обо мне", "Выполнил: Марков А.К. \n Группа: Т-409.13");
    petri::Check();
}

void MainWindow::initGrid() {
     lb1 = new QLabel("&Дата создания:");
     lb2 = new QLabel("&Дата доступа:");
     lb3 = new QLabel("&Дата изменения:");

     dte1 = new QDateTimeEdit;
     dte2 = new QDateTimeEdit;
     dte3 = new QDateTimeEdit;

     connect(dte1, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(Changed()));
     connect(dte2, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(Changed()));
     connect(dte3, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(Changed()));

     lb1->setBuddy(dte1);
     lb2->setBuddy(dte2);
     lb3->setBuddy(dte3);

     grid = new QGridLayout(this);

     grid->addWidget(lb1, 0, 0);
     grid->addWidget(dte1, 0, 1);

     grid->addWidget(lb2, 1, 0);
     grid->addWidget(dte2, 1, 1);

     grid->addWidget(lb3, 2, 0);
     grid->addWidget(dte3, 2, 1);

     for(int i = 0; i < 7; i++) {
         QCheckBox *item;
         QLabel *lbl;
         switch (i) {
            case 0: {
                 item = new QCheckBox(this);
                 lbl = new QLabel("&Архивированный", this);
                 break;
             }
             case 1: {
                 item = new QCheckBox(this);
                 lbl = new QLabel("&Скрытый", this);
                 break;
             }
             case 2: {
                 item = new QCheckBox(this);
                 lbl = new QLabel("&Неизвестный контент", this);
                 break;
             }
             case 3: {
                item = new QCheckBox(this);
                lbl = new QLabel("&Локальный", this);
                break;
             }
             case 4: {
                item = new QCheckBox(this);
                lbl = new QLabel("&Только чтение", this);
                break;
             }
             case 5: {
                item = new QCheckBox(this);
                lbl = new QLabel("&Системный", this);
                break;
             }
             case 6: {
                item = new QCheckBox(this);
                lbl = new QLabel("&Временный", this);
                break;
             }
         }
         lbl->setBuddy(item);
         connect(item, SIGNAL(stateChanged(int)), this, SLOT(Changed()));
         atrList.push_back(item);
         atrList[i]->setCheckState(Qt::Unchecked);
         grid->addWidget(lbl, i + 3, 0);
         grid->addWidget(item, i + 3, 1);
     }

     nameList = new QListWidget(this);
     grid->addWidget(nameList, 0, 3, 3, 3);
     connect(nameList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(ChangeName(QListWidgetItem*)));

     alterName = new QPushButton("&Вкл/Выкл", this);
     grid->addWidget(alterName, 0, 6);
     connect(alterName, SIGNAL(clicked()), this, SLOT(AlterName()));

     accessType = new QComboBox(this);
     accessType->addItem("Разрешить");
     accessType->addItem("Запретить");
     grid->addWidget(accessType, 1, 6);
     connect(accessType, SIGNAL(currentIndexChanged(int)), this, SLOT(AccessChange(int)));

     for (int i = 0; i < 14; i++) {
         QCheckBox *item;
         QLabel *lbl;
        switch (i) {
            case 0: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Полный доступ", this);
                break;
            }
            case 1: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Выполнение", this);
                break;
            }
            case 2: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Чтение", this);
                break;
            }
            case 3: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Чтение атрибутов", this);
                break;
            }
            case 4: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Чтение доп. атрибутов", this);
                break;
            }
            case 5: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Запись", this);
                break;
            }
            case 6: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Дополнение", this);
                break;
            }
            case 7: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Запись атрибутов", this);
                break;
            }
            case 8: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Запись доп. атрибутов", this);
                break;
            }
            case 9: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Удаление", this);
                break;
            }
            case 10: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Чтение прав доступа", this);
                break;
            }
            case 11: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Изменение прав доступа", this);
                break;
            }
            case 12: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Смена владельца", this);
                break;
            }
            case 13: {
                item =  new QCheckBox(this);
                lbl = new QLabel("&Синхронизация", this);
                break;
            }
        }
        rightList.push_back(item);
        rightList[i]->setCheckState(Qt::Unchecked);
        connect(item, SIGNAL(stateChanged(int)), this, SLOT(RightChanged(int)));
        lbl->setBuddy(item);
        if (i < 7) {
            grid->addWidget(lbl, i + 3, 3);
            grid->addWidget(item, i + 3, 4);
        } else {
            grid->addWidget(lbl, i + 3 -7, 5);
            grid->addWidget(item, i + 3 - 7, 6);
        }
     }
}

void MainWindow::setGridEnabled(bool enabled) {
    dte1->setEnabled(enabled);
    dte2->setEnabled(enabled);
    dte3->setEnabled(enabled);
    for (int i=0;i < atrList.size(); i++) {
        atrList[i]->setEnabled(enabled);
    }
    nameList->setEnabled(enabled);
    if (!enabled) {
        for (int i=0; i < rightList.size(); i++) {
            rightList[i]->setEnabled(enabled);
        }
        accessType->setEnabled(enabled);
        alterName->setEnabled(enabled);
        nameList->clear();
    }
}

MainWindow::~MainWindow() {}
