/*
  This file is part of YADSA (Yet Another Digit Software Archive) , a tool to search through the DIGIT CD/DVD softwares

  Version 0.2 beta
  Copyright (C) 2011  Nimesh Ghelani

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details:

  http://www.gnu.org/licenses/gpl.txt

  If you have any queries, suggestions, bug or feedback, feel free to mail me at nimeshghelani@gmail.com

*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <fstream>
#include <QListWidgetItem>


class QAction;
class QLabel;
class QPushButton;
class QLineEdit;
class QRadioButton;
class QComboBox;
class QGroupBox;
class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class mainWindow : public QMainWindow
{
    Q_OBJECT
    private:
    struct disc
    {
        short month;
        short year;
        char name[15];
        short count;
    }tmpDisc;
    class soft
    {
        public:
        char descrip[1000];
        char name[80];
        char path[200];
        char license[15];
        float size;
        void show();
    }tmpSoft;

    public:
    mainWindow();
    QString version;

    private slots:
    void changeTarget();
    void changeMode();
    void changeMonth(int );
    void changeYear(QString );
    void changeLicense(QString );
//    void onlineUpdate();
    bool offlineUpdate();
    void updateError();
    void showHelp();
    void about();
    void preSearchActivities();
    void showSoftDetails(QListWidgetItem *,QListWidgetItem *);

    private:
    char monthStr[13][10];
    char licenseStr[15][15];
    int licenseStrSize;
    int discSize,softSize;
    void createActions();
    void createMenus();
    void loadArchive();
    void performSearch();
    bool dbSearchContent(std::string);
    void str_tolower(std::string &,int );
    void addResult(int,int);

    std::vector<soft> softList;
    std::vector<disc> discList;
    char dbFilePath[200];

    QString searchKey, license;
    int target,mode,month,year;

    QWidget *centralWidget;

    QLabel *searchLabel;
    QLabel *optionsLabel;
    QLabel *targetLabel;
    QLabel *modeLabel;
    QLabel *monthLabel;
    QLabel *yearLabel;
    QLabel *licenseLabel;

    QLineEdit *searchBox;
    QGroupBox *optionsBox;
    QComboBox *monthComboBox;
    QComboBox *yearComboBox;
    QComboBox *licenseComboBox;
    QWidget *targetRadioArea;
    QWidget *modeRadioArea;
    QRadioButton *target0RadioButton;
    QRadioButton *target1RadioButton;
    QRadioButton *target2RadioButton;
    QRadioButton *mode0RadioButton;
    QRadioButton *mode1RadioButton;
    QPushButton *searchPushButton;

    QListWidget *searchResults;

    QLabel *softwareDetails;

    QHBoxLayout *mainLayout;
    QHBoxLayout *searchLayout;
    QHBoxLayout *targetLayout;
    QHBoxLayout *targetRadioLayout;
    QHBoxLayout *modeLayout;
    QHBoxLayout *modeRadioLayout;
    QHBoxLayout *monthLayout;
    QHBoxLayout *yearLayout;
    QHBoxLayout *licenseLayout;

    QVBoxLayout *mainLeftLayout;
    QVBoxLayout *optionsLayout;
    QVBoxLayout *mainRightLayout;

    QMenu *fileMenu;
    QMenu *updateMenu;
    QMenu *helpMenu;

    QAction *exitAction;
    QAction *onlineUpdateAction;
    QAction *offlineUpdateAction;
    QAction *helpAction;
    QAction *aboutAction;

    QWidget *space1;
};
#endif



