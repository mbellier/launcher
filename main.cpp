/***************************************************************************
**                                                                        **
**  This file is part of the program 'Launcher'.                          **
**  Copyright (C) 2013 Maxime Bellier                                     **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Maxime Bellier                                       **
**  Website/Contact: http://puissance.m4x.free.fr                         **
**             Date: 05.2013                                              **
****************************************************************************/

/*! \mainpage Launcher
 * This application uses few classes to manage Windows links and display them in a grid of buttons.
 * A database is used to store some statistics about the number of launches.
 * These statistics can be plotted thanks to the QCustomPlot widget by Emanuel Eichhammer from <http://www.WorksLikeClockwork.com/>.
 *
 * All the works are released under the GNU Public License.
 *
 * For further information, see http://puissance.m4x.free.fr/ (in French).
 *
 * Maxime Bellier <<maxime.bellier@gmx.fr>>.
 */

/*!
 * \file main.cpp
 * \brief Application entry point.
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \copyright This project is released under the GNU Public License.
 *
 * Initialises statistics and windows.
 *
 */

#include "launcher.h"
#include "statistics.h"
#include "statistics_dialog.h"
#include "settings.h"
#include <QtWidgets/QApplication>

#include <iostream>

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString settingsPath(app.applicationDirPath());
    settingsPath.append("/settings.ini");
    Settings settings(settingsPath);

    Statistics stats;
    StatisticsDialog statDialog(stats);

    Launcher launcher(stats, statDialog, settings);
    launcher.show();

    return app.exec();
}
