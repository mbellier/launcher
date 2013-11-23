#ifndef LAUNCHBUTTON_HPP
#define LAUNCHBUTTON_HPP

#include <QPushButton>
#include "link.h"
#include "statistics.h"
#include "settings.h"


class LaunchButton : public QPushButton
{
  Q_OBJECT
public:
  LaunchButton (QWidget *parent, Link &mLink, QIcon icon,
                Settings &settings, Statistics &stats,
                unsigned int nbLaunched = 0);
  unsigned int getNbLaunched() const;
  void contextMenuEvent(QContextMenuEvent *event);
  Link &link();
  QString text() const;
  void updateText(QString text);

public slots:
  void click();

private:
  static const unsigned int ELIDE_WIDTH = 44;
  Link &mLink;
  Statistics &stats;
  unsigned int nbLaunched;
};

//TODO ensure that parent is a Launcher
class LaunchButtonFactory {
public:
  LaunchButtonFactory(QWidget *parent, Settings &settings, Statistics &stats);
  LaunchButton *newLaunchButton(const QString &filePath) const;
private:
  QWidget *parent;
  Settings &settings;
  Statistics &stats;
};


#endif // LAUNCHBUTTON_HPP
