#include "launchbutton.hpp"
#include "launcher.h"
#include <QMenu>
#include <QDebug>

LaunchButton::LaunchButton (QWidget *parent, Link &link, QIcon icon, Settings &settings, Statistics &stats, unsigned int nbLaunched)
  : QPushButton(parent), mLink (link), stats(stats), nbLaunched(nbLaunched)
{
  // style
  setFlat(settings.settingflatButtons());
  setFixedSize(settings.settingButtonWidth(), settings.settingButtonHeight());

  // icon
  setIcon(icon);
  setIconSize(QSize(32,32));

  // name
  setStyleSheet("text-align:left;");
  updateText(mLink.linkName());

  // click event
  QObject::connect(this, SIGNAL(clicked()),
                   this, SLOT(click()));
}


void LaunchButton::updateText(QString text)
{
  QString elidedString = fontMetrics().elidedText(text, Qt::ElideRight, width()-ELIDE_WIDTH);
  setText(elidedString);
}

QString LaunchButton::text() const
{
  return mLink.linkName();
}

unsigned int LaunchButton::getNbLaunched() const
{
  return nbLaunched;
}


Link &LaunchButton::link()
{
  return mLink;
}

void LaunchButton::click()
{
  stats.addToken(text().toStdString());
  setText("Launching...");
  mLink.launch();
  QWidget *parent = (QWidget *)(this->parent()); // parent must be a QWidget, see constructor
  if (parent)
    parent->close();
}

void LaunchButton::contextMenuEvent(QContextMenuEvent *event)
{
  QWidget *parent = (QWidget *)(this->parent());
  if (parent)
    ((Launcher *)parent)->openContextMenu(event, this);
}

LaunchButtonFactory::LaunchButtonFactory(QWidget *parent, Settings &settings, Statistics &stats)
  : parent(parent), settings(settings), stats(stats)
{
}

LaunchButton *LaunchButtonFactory::newLaunchButton(const QString &filePath) const
{
  QIcon icon;
  Link *lnk = Link::loadLink(filePath, icon);
  if (lnk == NULL)
    return NULL;

  unsigned int nbLaunches = stats.nbTokens(lnk->linkName().toStdString());

  return new LaunchButton (parent, *lnk, icon,
                           settings, stats, nbLaunches);
}
