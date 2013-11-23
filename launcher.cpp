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

/*!
 * \file launcher.cpp
 * \brief Launcher window
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \copyright This project is released under the GNU Public License.
 */


#include "launcher.h"
#include "statistics_dialog.h"
#include "launchbutton.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSizePolicy>
#include <QCursor>
#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QFileInfo>
#include <QPoint>
#include <QUrl>

#include <algorithm>

using namespace std;

/*! \struct ButtonStats
 * \brief Structure used to sort the buttons by the number of launches.
 */
typedef struct {
  unsigned int button;
  unsigned int stats;
} ButtonStats;

/*! \brief Button sorting function
 * \param a Button
 * \param b Button
 * \return A boolean set to True if button 'a' stats are greater than 'b'.
 */
static bool sortButtonsFunc(const LaunchButton *a,
                            const LaunchButton *b){
  if (!a || !b)
    return false;
  if (a->getNbLaunched() == b->getNbLaunched())
    return a->text().compare(b->text()) < 0;
  return a->getNbLaunched() > b->getNbLaunched();
}




/*! \brief Launcher contructor
 * \param stats A Statistics object
 * \param statDialog Statistics dialog window
 * \param settings Settings dialog window
 */
Launcher::Launcher(Statistics &stats,
                   StatisticsDialog &statDialog,
                   Settings &settings)
  : QWidget(),
    m_buttons (this),
    m_contextMenuButton(NULL),
    m_stats(stats),
    m_statDialog(statDialog),
    m_settings(settings)
{
  m_maxColumnSize = m_settings.settingMaxColumnSize();
  m_path = QCoreApplication::applicationDirPath().append("/links/");

  // opening links folder
  QStringList filters;
  filters << "*.lnk" << "*.url";
  m_dir = new QDir(m_path);
  m_dir->setNameFilters(filters);

  if (!m_dir->exists(m_path) || m_dir->count() == 0)
  {
    m_dir->mkdir(m_path);
    QFile::copy(":/txt/README.txt", m_path + "README.txt");
    openLinksFolder();
  }

  // creation of the buttons
  LaunchButtonFactory buttonFactory( this, settings, stats);
  QList <LaunchButton *> buttonList;
  for (unsigned int k = 0; k < m_dir->count(); k++)
  {
    QString path = m_path + m_dir->entryList()[k];
    LaunchButton *button = buttonFactory.newLaunchButton(path);
    if (!button)
    {
      qDebug() << "[Error] Link unsupported: " << path;
      continue;
    }

    buttonList.append(button);
  }

  // sort buttons
  if (m_settings.settingSorted()){
    qSort(buttonList.begin(), buttonList.end(), sortButtonsFunc);
  }


  // position buttons
  m_columnSize = 0;
  unsigned int i = 0, j = 0;
  for (int k = 0; k < buttonList.count(); k++)
  {
    // add button to the grid
    m_buttons.addButton(buttonList.at(k), k);

    m_buttons.button(k)->move(settings.settingButtonWidth()  * j,
                              settings.settingButtonHeight() * i);
    i++;
    m_columnSize = (i > m_columnSize) ? i : m_columnSize;
    if (i >= m_maxColumnSize)
    {
      i = 0;
      j++;
    }
  }
  m_rowSize = (m_buttons.buttons().count() - 1) / (m_maxColumnSize) + 1;



  // Window parameters
  setPosition();
  setWindowFlags( Qt::CustomizeWindowHint ); // no title bar
  if (m_buttons.buttons().count() > 0)
    setFixedSize(m_rowSize * settings.settingButtonWidth(),
                 m_columnSize * settings.settingButtonHeight()); // fixed size
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  allowFocusLoss(false);
}

/*! \brief Launcher destructor
 * Saves statistics before quit
 */
Launcher::~Launcher()
{
  // NB: No need to free the buttons manually as they have a parent
  m_stats.save();
}

/*! \brief Returns the position of the windows taskbar.
 * \return 1 = vertical left,
 * \return 2 = horizontal up,
 * \return 3 = vertical right,
 * \return 0 = horizontal down,
 * \return -1 = unknown.
 */
int Launcher::getTaskbarPosition()
{
  QRect available = qApp->desktop()->availableGeometry();
  QRect total     = qApp->desktop()->geometry();

  if (available.topLeft().x() > total.topLeft().x())
    return 1;
  else if (available.topRight().x() < total.topRight().x())
    return 3;
  else if (available.bottomLeft().y() < total.bottomLeft().y())
    return 0;
  else if (available.topLeft().y() > total.topLeft().y())
    return 2;

  return -1;
}

/*! \brief Sets the position of the launcher window, using the taskbar position. */
void Launcher::setPosition()
{
  int taskbarPosition = getTaskbarPosition();
  int px, py;

  switch (taskbarPosition)
  {
  case 0:
  case 2:
    px = QCursor::pos().x();
    px -= m_settings.settingButtonWidth() / 2 - 14;
    if (px < 0) px = 0;
    break;
  case 3:
    px  = qApp->desktop()->availableGeometry().topRight().x();
    px -= m_rowSize * m_settings.settingButtonWidth();
    px -= 12; // window's border
    break;
  case 1:
    px  = qApp->desktop()->availableGeometry().topLeft().x();
    break;
  }

  switch (taskbarPosition)
  {
  case 1:
  case 3:
    py  = QCursor::pos().y();
    py -= m_settings.settingButtonHeight() / 2;
    if (py < 0) py = 0;
    break;
  case 0:
    py  = qApp->desktop()->availableGeometry().bottomLeft().y();
    py -= m_settings.settingButtonHeight() * m_columnSize;
    py -= 14; // window's border
    break;
  case 2:
    py  = qApp->desktop()->availableGeometry().topLeft().y();
    break;
  }

  this->move(px, py);
}


/*! \brief Used to (dis)allow focus loss of the launcher window
 * \param value
*/
void Launcher::allowFocusLoss(bool value)
{
  m_quitIfNoFocus = !value;
  if (m_quitIfNoFocus)
  {
    this->setFocus();
  }
}



/*! \brief Focus loss event
 * \param event
 * Quits if m_quitIfNoFocus is enabled
 */
void Launcher::focusOutEvent(QFocusEvent *event)
{
  if ((event->reason() != Qt::MouseFocusReason) &&
      (event->reason() != Qt::PopupFocusReason) &&
      (m_quitIfNoFocus))
  {
    this->close();
  }
  this->setFocus();
}

/*! \brief Keyboard event
 * \param event
 * Quits when Escape is pressed
 */
void Launcher::keyPressEvent (QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape)
  {
    this->close();
  }
}


void Launcher::contextMenuEvent(QContextMenuEvent *event)
{
  openContextMenu(event);
}

/*!
 * \brief Context menu
 * \param event
 */
void Launcher::openContextMenu(QContextMenuEvent *event, LaunchButton *button)
{
  QMenu menu(this);

  m_contextMenuButton = button;

  // if clicking on a button :
  if (button){
    // (1) Menu : Open file location
    QAction *actionFileLocation = menu.addAction("Open &file location");
    if (button->link().isUrl())
      actionFileLocation->setEnabled(false);
    connect(actionFileLocation, SIGNAL(triggered()),
            this, SLOT(openFileLocation()));

    // (2) Menu : Rename
    QAction *actionRename = menu.addAction("&Rename");
    connect(actionRename, SIGNAL(triggered()),
            this, SLOT(rename()));

    // (3) Separator
    menu.addSeparator();

    // (3b) Text
    menu.addAction(button->text())->setDisabled(true);

    // (4) Menu : nb launched
    int nbLaunched = button->getNbLaunched();
    QString stat;
    stat = "Launched " + QString::number(nbLaunched) + " time";
    if (nbLaunched > 1) stat += "s";
    menu.addAction(stat)->setDisabled(true);
  }

  // (5) Menu : separator
  menu.addSeparator();

  // (6) Menu : Statistics
  QAction *actionStatistics = menu.addAction("S&tatistics");
  connect(actionStatistics, SIGNAL(triggered()),
          this, SLOT(openStatistics()));

  // (7) Menu : Settings
  QAction *actionSettings = menu.addAction("&Settings");
  connect(actionSettings, SIGNAL(triggered()),
          this, SLOT(openSettings()));

  // (8) Menu : Open links folder
  QAction *actionLinksFolder = menu.addAction("Open &links folder");
  connect(actionLinksFolder, SIGNAL(triggered()),
          this, SLOT(openLinksFolder()));

  menu.exec(event->globalPos());
}

/*!
 * \brief Opens the folder containing the links parsed by the launcher.
 * Opens the windows explorer and quits.
 */
void Launcher::openLinksFolder()
{
  QDesktopServices::openUrl(QUrl("file:///" + m_path,
                                 QUrl::TolerantMode));
  this->close();
}

/*!
 * \brief Shows the statistics dialog.
 */
void Launcher::openStatistics()
{
  allowFocusLoss(true);
  m_statDialog.show();
  this->close();
}

/*! \brief Shows the settings dialog. */
void Launcher::openSettings()
{
  allowFocusLoss(true);
  m_settings.show();
  this->close();
}

/*! \brief Opens the folder pointed by a link.
 * Uses the link selected with the contextual menu.
 */
void Launcher::openFileLocation()
{
  if (m_contextMenuButton)
  m_contextMenuButton->link().openFileLocation();
  this->close();
}

/*!
 * \brief Renames a link and modifies the statistics database accordingly.
 */
void Launcher::rename()
{
  if (!m_contextMenuButton)
    return;

  allowFocusLoss(true);

  bool okButton;
  Link &lnk =  m_contextMenuButton->link();
  QString text = QInputDialog::getText(this, tr("Rename"),
                                       tr("New name :"),
                                       QLineEdit::Normal,
                                       lnk.linkName(),
                                       &okButton);
  if (okButton && !text.isEmpty())
  {
    m_stats.renameElement(lnk.linkName().toStdString(),
                          text.toStdString());
    m_stats.save();
    lnk.renameLink(text);
    m_contextMenuButton->updateText(text);
  }

  allowFocusLoss(false);
}



