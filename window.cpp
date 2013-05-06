/**
 * \file window.cpp
 * \brief Main window
 * \author Maxime B.
 * \version 0.1
 * \date 2013
 * \license{This project is released under the GNU Public License.}
 *
 * Main window displaying the grid of buttons.
 *
 */


#include "window.h"
#include "statistics.h"

#include <QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDir>
#include <iostream>
#include <fstream>
#include <QDesktopServices>
#include <QUrl>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QCursor>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QMenu>
#include <QPoint>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <algorithm>

#include <iostream>

using namespace std;

typedef struct {
  unsigned int button;
  unsigned int stats;
} ButtonStats;


static bool sortButtonsFunc(const ButtonStats &a, const ButtonStats &b){
  return a.stats > b.stats;
}


Window::Window(Statistics &stats)
  : QWidget(),
    m_button_size_x(150),
    m_button_size_y(60),
    m_max_column_size(8),
    m_context_menu_button_id(-1),
    m_stats(stats)
{

  m_buttons = new QButtonGroup(this);
  QObject::connect(m_buttons,
                   SIGNAL(buttonClicked(int)),
                   this,
                   SLOT(click(int)));

  path = QCoreApplication::applicationDirPath().append("/links/").toStdString();

  QStringList filters;
  filters << "*.lnk" << "*.url";
  m_dir = new QDir(QString::fromStdString(path));
  m_dir->setNameFilters(filters);

  if (!m_dir->exists(QString::fromStdString(path))) {
    m_dir->mkdir(QString::fromStdString(path));
    QFile::copy(QString::fromStdString(":/txt/README.txt"),
                QString::fromStdString(path + "README.txt"));

  }
  if (m_dir->count()==0) {
    openLinksFolder();
  }

  vector<ButtonStats> button_stats;
  for (unsigned int k = 0; k < m_dir->count(); k++) {
    QPushButton *button = new QPushButton(this);
    m_buttons->addButton(button,k);

    QString s = QString::fromStdString(path) + m_dir->entryList()[k];
    Link *lnk = launchButton(*button, s);
    m_links.push_back(lnk);

    ButtonStats bstats;
    bstats.button = k;
    bstats.stats= stats.nbTokens(button->text().toStdString());
    button_stats.push_back(bstats);
  }

  sort(button_stats.begin(), button_stats.end(), sortButtonsFunc);
  cerr << "SORTED" << endl;
  for (unsigned int l = 0; l < button_stats.size(); l++){
    cerr << button_stats[l].button << " : " << button_stats[l].stats << endl;
  }

  unsigned int i = 0, j = 0;
  m_column_size = 0;
  for (unsigned int k = 0; k < m_dir->count(); k++){
    unsigned int sorted_index = button_stats[k].button;
    m_buttons->button(sorted_index)->move(m_button_size_x * j,
                                          m_button_size_y * i);
    i++;
    m_column_size = (i > m_column_size) ? i : m_column_size;
    if (i >= m_max_column_size) {
      i = 0;
      j++;
    }
  }
  m_row_size = (m_dir->count() - 1) / (m_max_column_size) + 1;

  /* Window parameters */
  setPosition();
  setWindowFlags( Qt::CustomizeWindowHint ); // no title bar
  setFixedSize(m_row_size * m_button_size_x,
               m_column_size * m_button_size_y); // fixed size
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
}

/* Taskbar position :
 *  1 = vertical left,
 *  2 = horizontal up,
 *  3 = vertical right,
 *  0 = horizontal down,
 * -1 = unknown
 */
int Window::getTaskbarPosition() {
  QRect available = qApp->desktop()->availableGeometry();
  QRect total     = qApp->desktop()->geometry();

  if (available.topLeft().x() > total.topLeft().x()) {
    return 1;
  }
  else if (available.topRight().x() < total.topRight().x()){
    return 3;
  }
  else if (available.bottomLeft().y() < total.bottomLeft().y()) {
    return 0;
  }
  else if (available.topLeft().y() > total.topLeft().y()) {
    return 2;
  }

  return -1;
}


void Window::setPosition() {
  int taskbarPosition = getTaskbarPosition();

  int px, py;

  switch (taskbarPosition) {
  case 0:
  case 2:
    px = QCursor::pos().x();
    px -= m_button_size_x / 2 - 14;
    if (px < 0) px = 0;
    break;
  case 3:
    px  = qApp->desktop()->availableGeometry().topRight().x();
    px -= m_row_size * m_button_size_x;
    px -= 12; // window's border
    break;
  case 1:
    px  = qApp->desktop()->availableGeometry().topLeft().x();
    break;
  }

  switch (taskbarPosition) {
  case 1:
  case 3:
    py  = QCursor::pos().y();
    py -= m_button_size_y / 2;
    if (py < 0) py = 0;
    break;
  case 0:
    py  = qApp->desktop()->availableGeometry().bottomLeft().y();
    py -= m_button_size_y * m_column_size;
    py -= 14; // window's border
    break;
  case 2:
    py  = qApp->desktop()->availableGeometry().topLeft().y();
    break;
  }

  this->move(px, py);
}

void Window::click(int id){
  m_stats.addToken(m_buttons->button(id)->text().toStdString());
  m_buttons->button(id)->setText("Launching...");
  ((Link *)m_links[id])->launch();

  emit(quit());
}

void Window::quit() {
  m_stats.save();

  qApp->quit();

}

void Window::allowFocusLoss(bool value) {
  m_quitIfNoFocus = !value;
  if (m_quitIfNoFocus)
  {
    this->setFocus();
  }
}

Link *Window::loadSymLink(const QString &filePath, QIcon &icon) const {
  Link *l = new Link();
  QString lnk;
  QFileInfo fileInfo(filePath);
  QFileIconProvider iconExtractor;
  l->linkName(fileInfo.completeBaseName());
  l->linkDir(fileInfo.absoluteDir().absolutePath());
  lnk = fileInfo.symLinkTarget();
  fileInfo.setFile(lnk);
  QIcon test(iconExtractor.icon(fileInfo));
  icon = test;
  l->dirPath(fileInfo.absoluteDir().absolutePath());
  l->fileName(fileInfo.fileName());
  l->isUrl(false);
  return l;
}

Link *Window::loadURLLink(const QString &filePath, QIcon &icon) const {
  Link *lnk = new Link();
  QFileInfo fileInfo;
  fileInfo.setFile(filePath);
  lnk->linkName(fileInfo.completeBaseName());
  lnk->linkDir(fileInfo.absoluteDir().absolutePath());

  string line, ico;
  ifstream myfile (filePath.toStdString());
  if ( myfile.is_open() ) {
    while ( myfile.good() ) {
      getline (myfile,line);
      if ( line.substr(0,4) == "URL=" ) {
        lnk->url(QString::fromStdString(line.substr(4,line.length())));
      }
      else if ( line.substr(0,9) == "IconFile=" ){
        ico = line.substr(9,line.length());
        icon.addFile( QString::fromStdString(ico) );
      }
    }
    myfile.close();
  }
  else {
    return NULL;
  }
  return lnk;
}

Link *Window::loadLink(const QString &filePath, QIcon &icon) const {
  string ext = filePath.toStdString().substr(filePath.length()-4,4);
  if (ext == ".lnk") {
    return loadSymLink(filePath, icon);
  }
  else if (ext == ".url") {
    return loadURLLink(filePath, icon);
  }
  return NULL;
}

void Window::setLaunchButton(QPushButton &button,
                             QString &name,
                             QIcon &icon) const {
  button.setStyleSheet("text-align:left");
  button.setIcon(icon);
  button.setIconSize(QSize(32,32));
  button.setText(name);
  button.setFixedSize(m_button_size_x, m_button_size_y);
}

Link *Window::launchButton(QPushButton &button, const QString &filePath) const {
  QIcon icon;
  Link *lnk = loadLink(filePath, icon);
  if (lnk == NULL) return NULL;
  setLaunchButton(button, lnk->linkName(), icon);
  return(lnk);
}

void Window::focusOutEvent(QFocusEvent *event){
  if (event->reason() != Qt::MouseFocusReason
      && event->reason() != Qt::PopupFocusReason
      && m_quitIfNoFocus) {
    emit(quit());
  }
  this->setFocus();
  //setWindowTitle(QString::number(event->reason()));
}

void Window::keyPressEvent ( QKeyEvent * event ) {
  if (event->key() == Qt::Key_Escape) {
    emit(quit());
  }
}

int Window::getButtonIdFromPos(const QPoint &pos){
  for (int i = 0; i < m_buttons->buttons().count(); i++){
    QAbstractButton *b = (m_buttons->button(i));
    if (abs(b->x() - pos.x()) < b->width()
        && abs(b->y() - pos.y()) < b->height()){
      return i;
    }
  }
  return -1;
}

void Window::contextMenuEvent(QContextMenuEvent *event) {
  QMenu menu(this);
  m_context_menu_button_id = getButtonIdFromPos(event->pos());
  if (m_context_menu_button_id >= 0) {
    QAction *actionFileLocation = menu.addAction("Open &file location");
    if (((Link *) m_links[m_context_menu_button_id])->isUrl()) {
      actionFileLocation->setEnabled(false);
    }
    connect(actionFileLocation,
            SIGNAL(triggered()),
            this,
            SLOT(openFileLocation()));

    QAction *actionRename = menu.addAction("&Rename");
    connect(actionRename, SIGNAL(triggered()), this, SLOT(rename()));
  }

  QAction *actionLinksFolder = menu.addAction("Open &links folder");
  connect(actionLinksFolder,
          SIGNAL(triggered()),
          this,
          SLOT(openLinksFolder()));
  menu.exec(event->globalPos());
}

void Window::openLinksFolder(){
  QDesktopServices::openUrl(QUrl(QString::fromStdString("file:///" + path),
                                 QUrl::TolerantMode));
  emit(quit());
}

void Window::openFileLocation(){
  ((Link *)m_links[m_context_menu_button_id])->openFileLocation();
}

void Window::rename(){
  allowFocusLoss(true);
  bool okButton;
  Link *lnk = ((Link *)m_links[m_context_menu_button_id]);
  QString text = QInputDialog::getText(this, tr("Rename"),
                                       tr("New name :"), QLineEdit::Normal,
                                       lnk->linkName(), &okButton);
  if (okButton && !text.isEmpty()) {
    m_stats.renameElement(lnk->linkName().toStdString(),
                          text.toStdString());
    m_stats.save();
    lnk->renameLink(text);
    m_buttons->button(m_context_menu_button_id)->setText(text);
  }

  allowFocusLoss(false);
}



