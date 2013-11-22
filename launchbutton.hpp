#ifndef LAUNCHBUTTON_HPP
#define LAUNCHBUTTON_HPP

#include <QPushButton>
#include "link.h"
#include "statistics.h"


class LaunchButton : public QPushButton
{
  Q_OBJECT
public:
  LaunchButton (QWidget *parent, Link &mLink, QIcon icon,
                unsigned int width, unsigned int height,
                Statistics &stats, unsigned int nbLaunched = 0);
  unsigned int getNbLaunched() const;
  void contextMenuEvent(QContextMenuEvent *event);
  Link &link();

public slots:
  void click();

private:
  Link &mLink;
  Statistics &stats;
  unsigned int nbLaunched;
};

//TODO ensure that parent is a Launcher
class LaunchButtonFactory {
public:
  LaunchButtonFactory(QWidget *parent, unsigned int buttonWidth, unsigned int buttonHeight, Statistics &stats);
  LaunchButton *newLaunchButton(const QString &filePath) const;
private:
  QWidget *parent;
  unsigned int width, height;
  Statistics &stats;
};


#endif // LAUNCHBUTTON_HPP
