#include "launchbutton.hpp"
#include "launcher.h"
#include <QMenu>

#include <QDebug>

LaunchButton::LaunchButton (QWidget *parent, Link &link, QIcon icon, unsigned int width, unsigned int height, Statistics &stats, unsigned int nbLaunched)
  : QPushButton(parent), mLink (link), stats(stats), nbLaunched(nbLaunched)
{
    // icon
    setIcon(icon);
    setIconSize(QSize(32,32));

    // name
    setStyleSheet("text-align:left");
    setText(mLink.linkName());
    setFixedSize(width, height);

    // click event
    QObject::connect(this, SIGNAL(clicked()),
                     this, SLOT(click()));
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

LaunchButtonFactory::LaunchButtonFactory(QWidget *parent, unsigned int buttonWidth, unsigned int buttonHeight, Statistics &stats)
  : parent(parent), width(buttonWidth), height(buttonHeight), stats(stats)
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
                             width, height,
                             stats, nbLaunches);
}
