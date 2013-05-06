/**
 * \file main.cpp
 * \brief Main
 * \author Maxime B.
 * \version 0.1
 * \date 2013
 * \license{This project is released under the GNU Public License.}
 *
 * Application initialisation.
 *
 */

#include "window.h"
#include "statistics_dialog.h"
#include "statistics.h"
#include <QtWidgets/QApplication>

Q_DECL_EXPORT int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Statistics stats;
    stats.sort();

//    Window window(stats);
//    window.show();

    StatisticsDialog statDiag(stats);
    statDiag.show();

    return app.exec();
}
