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
#include "mainWindow.h"
#include <QtGui>
#include <fstream>
#include <string>
void mainWindow::loadArchive()
{
    discList.clear();
    softList.clear();
    std::ifstream dbFile(dbFilePath,std::ios::in | std::ios::binary);
    if(!dbFile)
    {
        QMessageBox::critical(this,tr("YADSA"),tr("Archive File not found!!! Place it where it is supposed to be or reinstall!!<br /><br /><font color=red>Exiting!!</font>"));
        exit(1);
    }

    while(dbFile.read((char *) &tmpDisc, discSize))
    {
        discList.push_back(tmpDisc);
        for(int i=0;i<tmpDisc.count;i++)
        {
            dbFile.read((char *) &tmpSoft, softSize);
            softList.push_back(tmpSoft);
        }
    }
    dbFile.close();
}

void mainWindow::performSearch()
{
    disconnect(searchResults,SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *)),this,SLOT(showSoftDetails(QListWidgetItem *,QListWidgetItem *)));
    searchResults->clear();
    softwareDetails->setText("");
    connect(searchResults,SIGNAL(currentItemChanged(QListWidgetItem *,QListWidgetItem *)),this,SLOT(showSoftDetails(QListWidgetItem *,QListWidgetItem *)));
    searchKey=searchKey.toLower();
    QStringList keywordList=searchKey.split(QRegExp("\\s+"));
    std::vector<std::string> keywords;
    for (int i = 0; i < keywordList.size(); ++i)
    keywords.push_back(keywordList.at(i).toLocal8Bit().constData());

    int totalFound=0;
    int softIndex=0;
    int totalKeywords=keywords.size();
    for(int i=0;i<discList.size();i++)
    {
        tmpDisc=discList[i];
        if((year==tmpDisc.year || year==0) && (month==tmpDisc.month || month==0))
        {
            for(int k=softIndex;k<softIndex+tmpDisc.count;k++)
            {
                bool flag=false;
                bool found=false;
                int j=totalKeywords;
                tmpSoft=softList[k];
                flag=(QString(tmpSoft.license).toLower()==license.toLower() || license=="");

                while(j-- && flag)
                {
                    if(dbSearchContent(keywords[j]))
                    {
                         found=true;
                         if(mode==1)
                         break;
                    }
                    else
                    {
                         if(mode==0)
                         {
                              found=false;
                              break;
                         }
                    }
                }
                if(found)
                {
                    totalFound++;
                    addResult(k,i);
                }
            }
        }
        softIndex+=tmpDisc.count;

    }
    if(totalFound==0)
    {
        QMessageBox::warning(this,tr("Not Found!!!"),tr("No matches found in the archive"));
    }
}
bool mainWindow::dbSearchContent(std::string keyword)
{
    if(target==0 || target==1)
     {
     std::string name=std::string(tmpSoft.name);
     str_tolower(name,name.size());
     if(name.find(keyword)!=std::string::npos)
          return true;
     }

     if(target==0 || target==2)
     {
     std::string descrip=tmpSoft.descrip;
     str_tolower(descrip,descrip.size());
     if(descrip.find(keyword)!=std::string::npos)
          return true;
     }
     return false;
}
void mainWindow::str_tolower(std::string &str,int size)
{
     while(size--)
     str[size]=tolower(str[size]);
}
void mainWindow::addResult(int indexSoft,int indexDisc)
{
    QListWidgetItem *tmpListWidgetItem=new QListWidgetItem(tr(tmpSoft.name));
                searchResults->addItem(tmpListWidgetItem);
    tmpListWidgetItem->setData(1,QVariant(indexSoft));
    tmpListWidgetItem->setData(28,QVariant(indexDisc));
}
void mainWindow::showSoftDetails(QListWidgetItem *item, QListWidgetItem *tmpItem)
{
    int index=item->data(1).toInt();
    int indexDisc=item->data(28).toInt();
    QString textToSet=tr("<b>Name:</b> %1<br /><br /><b>Description:</b> %2<br /><br /><b>License:</b> %3 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Size:</b> %4MB<br /><b>Path:</b> %5<br /><br />").arg(softList[index].name).arg(softList[index].descrip).arg(softList[index].license).arg(softList[index].size).arg(softList[index].path);
    textToSet+= tr("<b>On:</b> %1 %2 %3").arg(discList[indexDisc].name).arg(monthStr[discList[indexDisc].month]).arg(discList[indexDisc].year);
    softwareDetails->setText(textToSet);
}
