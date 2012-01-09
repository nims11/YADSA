/*
  YADSA (Yet Another Digit Software Archive) a tool to search through the DIGIT CD/DVD softwares
  Version 0.2 beta

  Copyright (C) 2011 Nimesh Ghelani

  Archive Data data obtained through thinkdigit.com/forum FEEDBACK section.

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
#include <QApplication>
#include "./mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    mainWindow *window=new mainWindow;
    window->show();
    return app.exec();
}
