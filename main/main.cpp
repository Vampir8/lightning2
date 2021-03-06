/************************************************************************
DSPatcher - Cross-Platform Graphical Patching Tool For DSPatch
Copyright (c) 2014-2015 Marcus Tomlinson

This file is part of DSPatcher.

GNU Lesser General Public License Usage
This file may be used under the terms of the GNU Lesser General Public
License version 3.0 as published by the Free Software Foundation and
appearing in the file LGPLv3.txt included in the packaging of this
file. Please review the following information to ensure the GNU Lesser
General Public License version 3.0 requirements will be met:
http://www.gnu.org/copyleft/lgpl.html.

Other Usage
Alternatively, this file may be used in accordance with the terms and
conditions contained in a signed written agreement between you and
Marcus Tomlinson.

DSPatch is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
************************************************************************/

#include <DsprController.h>
#include <QtpMain.h>

#include <QApplication>

int main(int argv, char* args[])
{
    QApplication app(argv, args);
    QtpMain mainWindow;
    mainWindow.show();

    DsprController controller(mainWindow);

    return app.exec();
}
