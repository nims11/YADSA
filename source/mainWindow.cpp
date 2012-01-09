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
#include <QtGui>
#include <cstring>
#include "mainWindow.h"

mainWindow::mainWindow()
{
    strcpy(dbFilePath,"archive.yadsa");
    version="0.2 beta";
    setWindowIcon(QIcon(":/images/logo.png"));
    discSize=sizeof(disc);
    softSize=sizeof(soft);
    strcpy(monthStr[0],"");
    strcpy(monthStr[1],"January");
    strcpy(monthStr[2],"February");
    strcpy(monthStr[3],"March");
    strcpy(monthStr[4],"April");
    strcpy(monthStr[5],"May");
    strcpy(monthStr[6],"June");
    strcpy(monthStr[7],"July");
    strcpy(monthStr[8],"August");
    strcpy(monthStr[9],"September");
    strcpy(monthStr[10],"October");
    strcpy(monthStr[11],"November");
    strcpy(monthStr[12],"December");
    strcpy(licenseStr[0],"");
    strcpy(licenseStr[1],"Free");
    strcpy(licenseStr[2],"Trial");
    strcpy(licenseStr[3],"Demo");
    strcpy(licenseStr[4],"Shareware");
    strcpy(licenseStr[5],"Licensed");
    licenseStrSize=6;
    loadArchive();
    centralWidget=new QWidget;

    searchLabel=new QLabel(tr("Search Term(s):"));
    searchBox=new QLineEdit;
    searchLabel->setBuddy(searchBox);

    optionsBox=new QGroupBox(tr("Options"));

    targetLabel=new QLabel(tr("Target:"));
    targetRadioArea=new QWidget;
    target0RadioButton=new QRadioButton(tr("Both"),targetRadioArea);
    target0RadioButton->setChecked(true);
    target1RadioButton=new QRadioButton(tr("Name"),targetRadioArea);
    target2RadioButton=new QRadioButton(tr("Description"),targetRadioArea);
    connect(target0RadioButton,SIGNAL(clicked()),this,SLOT(changeTarget()));
    connect(target1RadioButton,SIGNAL(clicked()),this,SLOT(changeTarget()));
    connect(target2RadioButton,SIGNAL(clicked()),this,SLOT(changeTarget()));

    modeLabel=new QLabel(tr("Mode:"));
    modeRadioArea=new QWidget;
    mode0RadioButton=new QRadioButton(tr("Intersection"),modeRadioArea);
    mode0RadioButton->setChecked(true);
    mode1RadioButton=new QRadioButton(tr("Union"),modeRadioArea);
    connect(mode0RadioButton,SIGNAL(clicked()),this,SLOT(changeMode()));
    connect(mode1RadioButton,SIGNAL(clicked()),this,SLOT(changeMode()));

    monthLabel=new QLabel(tr("Month:"));
    monthComboBox=new QComboBox;
    monthComboBox->addItem(tr("All"));
    for(int i=1;i<13;i++)
    monthComboBox->addItem(tr(monthStr[i]));
    connect(monthComboBox,SIGNAL(currentIndexChanged(int )),this,SLOT(changeMonth(int )));

    yearLabel=new QLabel(tr("Year:"));
    yearComboBox=new QComboBox;
    yearComboBox->addItem(tr("All"));
    yearComboBox->addItem(tr("2005"));
    yearComboBox->addItem(tr("2006"));
    yearComboBox->addItem(tr("2007"));
    yearComboBox->addItem(tr("2008"));
    yearComboBox->addItem(tr("2009"));
    yearComboBox->addItem(tr("2010"));
    yearComboBox->addItem(tr("2011"));
    connect(yearComboBox,SIGNAL(currentIndexChanged(QString )),this,SLOT(changeYear(QString )));

    licenseLabel=new QLabel(tr("License:"));
    licenseComboBox=new QComboBox;
    licenseComboBox->addItem(tr("All"));
    for(int i=1;i<licenseStrSize;i++)
    licenseComboBox->addItem(tr(licenseStr[i]));
    connect(licenseComboBox,SIGNAL(currentIndexChanged(QString )),this,SLOT(changeLicense(QString )));

    searchPushButton=new QPushButton(tr("Search"));
    searchPushButton->setDefault(true);
    connect(searchPushButton,SIGNAL(clicked()),this,SLOT(preSearchActivities()));
    //searchPushButton->setEnabled(false);

//setting up layouts
    searchLayout=new QHBoxLayout;
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchBox);

    targetLayout=new QHBoxLayout;
    targetRadioLayout=new QHBoxLayout;
    targetLayout->addWidget(targetLabel);
    targetRadioLayout->addWidget(target0RadioButton);
    targetRadioLayout->addWidget(target1RadioButton);
    targetRadioLayout->addWidget(target2RadioButton);
    targetRadioArea->setLayout(targetRadioLayout);
    targetLayout->addWidget(targetRadioArea);
    targetLayout->addStretch();

    modeLayout=new QHBoxLayout;
    modeRadioLayout=new QHBoxLayout;
    modeLayout->addWidget(modeLabel);
    modeRadioLayout->addWidget(mode0RadioButton);
    modeRadioLayout->addWidget(mode1RadioButton);
    modeRadioArea->setLayout(modeRadioLayout);
    modeLayout->addWidget(modeRadioArea);
    modeLayout->addStretch();

    monthLayout=new QHBoxLayout;
    monthLayout->addWidget(monthLabel);
    monthLayout->addWidget(monthComboBox);

    yearLayout=new QHBoxLayout;
    yearLayout->addWidget(yearLabel);
    yearLayout->addWidget(yearComboBox);

    licenseLayout=new QHBoxLayout;
    licenseLayout->addWidget(licenseLabel);
    licenseLayout->addWidget(licenseComboBox);

    optionsLayout=new QVBoxLayout;
    optionsLayout->setSpacing(1);
    optionsLayout->addLayout(targetLayout);
    optionsLayout->addLayout(modeLayout);
    optionsLayout->addLayout(monthLayout);
    optionsLayout->addLayout(yearLayout);
    optionsLayout->addLayout(licenseLayout);

    optionsBox->setLayout(optionsLayout);

    mainLeftLayout=new QVBoxLayout;
    mainLeftLayout->setSpacing(35);
    mainLeftLayout->addLayout(searchLayout);
    mainLeftLayout->addWidget(optionsBox);
    space1=new QWidget();
    space1->setFixedHeight(30);
    mainLeftLayout->addWidget(space1);
    mainLeftLayout->addWidget(searchPushButton);

    searchResults=new QListWidget;
    connect(searchResults,SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *)),this,SLOT(showSoftDetails(QListWidgetItem *,QListWidgetItem *)));
    softwareDetails=new QLabel(tr("                                                                                        "));
    softwareDetails->setFixedWidth(softwareDetails->sizeHint().width());
    softwareDetails->setWordWrap(true);
    mainRightLayout=new QVBoxLayout;
    mainRightLayout->addWidget(softwareDetails);
    mainRightLayout->addStretch();

    mainLayout=new QHBoxLayout;
    mainLayout->addLayout(mainLeftLayout);
    mainLayout->addWidget(searchResults);
    mainLayout->addLayout(mainRightLayout);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);
    setWindowTitle("YADSA by Nimesh Ghelani");
    setFixedHeight(sizeHint().height()+30);
    setFixedWidth(sizeHint().width());

    createActions();
    createMenus();

    searchKey="";
    license="";
    target=0;
    mode=0;
    month=0;
    year=0;
}

void mainWindow::createActions()
{
    exitAction=new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    onlineUpdateAction=new QAction(tr("O&nline Update"),this);
    onlineUpdateAction->setEnabled(false);
    //connect(onlineUpdateAction, SIGNAL(triggered()), this, SLOT(onlineUpdate()));

    offlineUpdateAction=new QAction(tr("O&ffline Update"),this);
    connect(offlineUpdateAction, SIGNAL(triggered()), this, SLOT(offlineUpdate()));

    helpAction=new QAction(tr("&Help"),this);
    helpAction->setShortcut(tr("Ctrl+F1"));
    connect(helpAction,SIGNAL(triggered()), this, SLOT(showHelp()));

    aboutAction=new QAction(tr("&About"),this);
    connect(aboutAction,SIGNAL(triggered()), this, SLOT(about()));
}

void mainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    updateMenu=menuBar()->addMenu(tr("&Update"));
    updateMenu->addAction(onlineUpdateAction);
    updateMenu->addAction(offlineUpdateAction);

    helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
}

void mainWindow::preSearchActivities()
{
    QString tmp=searchBox->text();
    int effectiveSize=0;
    int size=tmp.size();
    for(int i=0;i<size && effectiveSize<2;i++)
    {
        if(tmp[i]!=' ')
        effectiveSize++;
    }

    if(effectiveSize<2)
    {
        QMessageBox::critical(this,tr("YADSA"),tr("Search term should have atleast 2 characters(excluding spaces)"));
    }else
    {
        searchKey=tmp;
        performSearch();
    }
}

void mainWindow::changeTarget()
{
    if(target0RadioButton->isChecked())
    target=0;
    else if(target1RadioButton->isChecked())
    target=1;
    else if(target2RadioButton->isChecked())
    target=2;
}
void mainWindow::changeMode()
{
    if(mode0RadioButton->isChecked())
    mode=0;
    else if(mode1RadioButton->isChecked())
    mode=1;
}
void mainWindow::changeMonth(int newMonth)
{
    month=newMonth;
}
void mainWindow::changeYear(QString newYear)
{
    if(newYear=="All")
    year=0;
    else
    year=newYear.toInt();
}
void mainWindow::changeLicense(QString newLicense)
{
    if(newLicense=="All")
    license="";
    else
    license=newLicense;
}
void mainWindow::about()
{
    QMessageBox::about(this, tr("About YADSA"),
                       tr("<h2>YADSA %1</h2>"
                          "Copyright &copy; 2011 Nimesh Ghelani"
                          "<br /><br />YADSA (Yet Another Digit Software Archive) is an application that helps digit readers search through the digit DVD contents"
                          "<br /><br />Total Softwares in the Archive : %2<br />From July 2005 to %3 %4").arg(version).arg(softList.size()).arg(monthStr[discList[discList.size()-1].month]).arg(discList[discList.size()-1].year));
}
void mainWindow::showHelp()
{
    QMessageBox::about(this, tr("Help"),tr("<b>Target:</b> Specifies which attribute of the Software is searched<br /><br />"
                                           "<b>Mode: </b><br />Intersection - A software is included in the search result if atleast one of the keyword is found in its attributes<br />Union: A software is included in the search result only if all the keywords are found in the software attributes<br /><br />"
                                           "Any Problems, Bugs, feedbacks; mail me at nimeshghelani@gmail.com"));
}
