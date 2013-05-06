#ifndef WINDOW_H
#define WINDOW_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QIcon>
#include <QDir>

#include <string>
#include <vector>
#include <QFocusEvent>

#include "link.h"
#include "statistics.h"


class Window : public QWidget {
    Q_OBJECT
public:
    Window(Statistics &m_stats);
    void setLaunchButton(QPushButton &button, QString &name, QIcon &icon) const;
    int Window::getTaskbarPosition();
    void setPosition();
    void contextMenuEvent(QContextMenuEvent *event);
    int getButtonIdFromPos(const QPoint &pos);
    Link *launchButton(QPushButton &button, const QString &filePath) const;
    Link *loadLink(const QString &filePath, QIcon &icon) const;
    Link *loadURLLink(const QString &filePath, QIcon &icon) const;
    Link *loadSymLink(const QString &filePath, QIcon &icon) const;
    void allowFocusLoss(bool value);

public slots:
    void openFileLocation();
    void openLinksFolder();
    void click(int);
    void quit();

    void focusOutEvent(QFocusEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void rename();

private:
    QButtonGroup *m_buttons;
    std::string  path;
    bool m_quitIfNoFocus;
    unsigned int m_button_size_x;
    unsigned int m_button_size_y;
    unsigned int m_column_size;
    unsigned int m_max_column_size;
    unsigned int m_row_size;
    int m_context_menu_button_id;
    std::vector<Link *> m_links;
    QDir *m_dir;
    Statistics &m_stats;
};

#endif // WINDOW_H
