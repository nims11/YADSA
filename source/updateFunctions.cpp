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

bool mainWindow::offlineUpdate()
{
    QString updateFileName = QFileDialog::getOpenFileName(this,
                                   tr("Select Update File"), ".",
                                   tr("yadsa update file (*.yadsa)"));
    if(updateFileName.isEmpty())
    return true;
    std::ifstream updateFile(qPrintable(updateFileName),std::ios::in | std::ios::binary);
    std::ofstream targetFile(dbFilePath,std::ios::in | std::ios::binary | std::ios::app);
    if(!updateFile || !targetFile)
    {
        updateError();
        return false;
    }
    updateFile.seekg(0,std::ios::end);
    if(updateFile.tellg()==0)
    {
        updateError();
        return false;
    }
    updateFile.seekg(0);
    //to add check for confirming valid update file
    disc a;
    while(updateFile.read((char *) &a, sizeof(a)))
               {
                    soft b;
                    targetFile.write((char *) &a, sizeof(a));
                    for(int i=0;i<a.count;i++)
                    {
                         updateFile.read((char *) &b, sizeof(b));
                         targetFile.write((char *) &b, sizeof(b));
                    }
               }

    updateFile.close();
    targetFile.close();
    QMessageBox::information(this,tr("Success"),tr("Update successful<br />Reloading Archive..."));
    loadArchive();
    return true;
}
void mainWindow::updateError()
{
    QMessageBox::critical(this,tr("ERROR!!"),tr("ERROR DURING UPDATE! Bad update File or corrupted archive"));
}
