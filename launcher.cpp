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
static bool sortButtonsFunc(const ButtonStats &a,
                            const ButtonStats &b){
  return a.stats > b.stats;
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
    m_buttonWidth(150),
    m_buttonHeight(60),
    m_contextMenuButtonId(-1),
    m_stats(stats),
    m_statDialog(statDialog),
    m_settings(settings)
{
  m_maxColumnSize = m_settings.settingMaxColumnSize();
  m_path = QCoreApplication::applicationDirPath().append("/links/");

  m_buttons = new QButtonGroup(this);
  QObject::connect(m_buttons, SIGNAL(buttonClicked(int)),
                   this, SLOT(click(int)));

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

  vector<ButtonStats> button_stats;
  for (unsigned int k = 0; k < m_dir->count(); k++)
  {
    QPushButton *button = new QPushButton(this);
    QString s = m_path + m_dir->entryList()[k];

    Link *lnk = launchButton(*button, s);
    if (!lnk)
    {
      qDebug() << "[Error] Link unsupported: " << s;
      button->close();
      delete button;
      continue;
    }

    m_buttons->addButton(button,k);
    m_links.push_back(lnk);

    ButtonStats bstats;
    bstats.button = k;
    bstats.stats= stats.nbTokens(button->text().toStdString());
    button_stats.push_back(bstats);
  }

  // button sorting
  if (m_settings.settingSorted()){
    sort(button_stats.begin(), button_stats.end(), sortButtonsFunc);
    // TODO This is a quick fix - better make a class for launchButtons !
    std::vector<Link *> tmp;
    for (unsigned int k = 0; k < m_links.size(); k++){
      unsigned int sorted_index = button_stats[k].button;
      tmp.push_back(m_links[sorted_index]);
    }
    m_links = tmp;
  }

  // position buttons
  unsigned int i = 0, j = 0;
  m_columnSize = 0;
  for (int k = 0; k < m_buttons->buttons().count(); k++)
  {
    unsigned int sorted_index = button_stats[k].button;
    m_buttons->button(sorted_index)->move(m_buttonWidth * j,
                                          m_buttonHeight * i);
    i++;
    m_columnSize = (i > m_columnSize) ? i : m_columnSize;
    if (i >= m_maxColumnSize)
    {
      i = 0;
      j++;
    }
  }
  m_rowSize = (m_buttons->buttons().count() - 1) / (m_maxColumnSize) + 1;

  // Window parameters
  setPosition();
  setWindowFlags( Qt::CustomizeWindowHint ); // no title bar
  if (m_buttons->buttons().count() > 0)
    setFixedSize(m_rowSize * m_buttonWidth,
                 m_columnSize * m_buttonHeight); // fixed size
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  allowFocusLoss(false);
}

/*! \brief Launcher destructor
 * Saves statistics before quit
 */
Launcher::~Launcher()
{
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
    px -= m_buttonWidth / 2 - 14;
    if (px < 0) px = 0;
    break;
  case 3:
    px  = qApp->desktop()->availableGeometry().topRight().x();
    px -= m_rowSize * m_buttonWidth;
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
    py -= m_buttonHeight / 2;
    if (py < 0) py = 0;
    break;
  case 0:
    py  = qApp->desktop()->availableGeometry().bottomLeft().y();
    py -= m_buttonHeight * m_columnSize;
    py -= 14; // window's border
    break;
  case 2:
    py  = qApp->desktop()->availableGeometry().topLeft().y();
    break;
  }

  this->move(px, py);
}

/*! \brief Button click event
 * \param id Refers to the clicked button
 */
void Launcher::click(int id)
{
  m_stats.addToken(m_buttons->button(id)->text().toStdString());
  m_buttons->button(id)->setText("Launching...");
  ((Link *)m_links[id])->launch();
  this->close();
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

/*! \brief Sets up a QPushButton as a launch button given a name and an icon
 * \param button Button to set up
 * \param name Application name
 *  \param icon Application icon
 */
void Launcher::setLaunchButton(QPushButton &button,
                               QString &name,
                               QIcon &icon) const
{
  button.setStyleSheet("text-align:left");
  button.setIcon(icon);
  button.setIconSize(QSize(32,32));
  button.setText(name);
  button.setFixedSize(m_buttonWidth, m_buttonHeight);
}

/*! \brief Sets up a QPushButton as a launch button given a file path
 * \param button Button to set up
 * \param filePath path to the link
 * \return A Link object
 */
Link *Launcher::launchButton(QPushButton &button,
                             const QString &filePath) const
{
  QIcon icon;
  Link *lnk = Link::loadLink(filePath, icon);
  if (lnk == NULL) return NULL;
  setLaunchButton(button, lnk->linkName(), icon);
  return(lnk);
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

/*!
 * \brief Maps a position on the button grid to the id of the corresponding button.
 * \param pos Mouse position
 * \return id referring to the button selected
 * \return -1 if none selected
 */
int Launcher::getButtonIdFromPos(const QPoint &pos)
{
  for (int i = 0; i < m_buttons->buttons().count(); i++)
  {
    QAbstractButton *b = (m_buttons->button(i));
    if ((abs(b->x() - pos.x()) < b->width()) &&
        (abs(b->y() - pos.y()) < b->height())){
      return i;
    }
  }
  return -1;
}

/*!
 * \brief Context menu
 * \param event
 */
void Launcher::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu menu(this);
  m_contextMenuButtonId = getButtonIdFromPos(event->pos());

  // if clicking on a button :
  if (m_contextMenuButtonId >= 0){
    // (1) Menu : Open file location
    QAction *actionFileLocation = menu.addAction("Open &file location");
    if (((Link *) m_links[m_contextMenuButtonId])->isUrl())
      actionFileLocation->setEnabled(false);
    connect(actionFileLocation, SIGNAL(triggered()),
            this, SLOT(openFileLocation()));

    // (2) Menu : Rename
    QAction *actionRename = menu.addAction("&Rename");
    connect(actionRename, SIGNAL(triggered()),
            this, SLOT(rename()));

    // (3) Separator
    menu.addSeparator();

    // (4) Menu : nb launched
    int nbLaunched = m_stats.nbTokens(m_buttons->
                                      button(m_contextMenuButtonId)->
                                      text().toStdString());
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
  qDebug() << m_contextMenuButtonId;
  ((Link *)m_links[m_contextMenuButtonId])->openFileLocation();
  this->close();
}

/*!
 * \brief Renames a link and modifies the statistics database accordingly.
 */
void Launcher::rename()
{
  allowFocusLoss(true);

  bool okButton;
  Link *lnk = ((Link *)m_links[m_contextMenuButtonId]);
  QString text = QInputDialog::getText(this, tr("Rename"),
                                       tr("New name :"),
                                       QLineEdit::Normal,
                                       lnk->linkName(),
                                       &okButton);
  if (okButton && !text.isEmpty())
  {
    m_stats.renameElement(lnk->linkName().toStdString(),
                          text.toStdString());
    m_stats.save();
    lnk->renameLink(text);
    m_buttons->button(m_contextMenuButtonId)->setText(text);
  }

  allowFocusLoss(false);
}



