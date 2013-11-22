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

#ifndef LINK_H_
#define LINK_H_

#include <QString>
#include <QIcon>

/*!
 * \brief The Link class
 *
 * Contains parsed informations from a link file.
 *
 */
class Link {
public:
    Link();
    Link(const QString &linkName, const QString &linkDir,
         const QString &dirPath, const QString &fileName);
    Link(const QString &linkName, const QString &linkDir,
         const QString &url);

    // factory
    static Link *loadLink(const QString &filePath, QIcon &icon);
    static Link *loadURLLink(const QString &filePath, QIcon &icon);
    static Link *loadSymLink(const QString &filePath, QIcon &icon);

    // methods
    QString link() const;
    void launch();
    bool openFileLocation();
    QString linkExtension() const;
    void renameLink(const QString &newLinkName);

    // accessors
    QString filePath() const;
    QString fileName() const;
    void fileName(const QString &fileName);
    QString dirPath() const;
    QString linkName() const;
    void linkName(const QString &linkName);
    QString url() const;
    void url(const QString &url);
    bool isUrl() const;
    void isUrl(const bool &value);
    void linkDir(const QString &linkDir);
    QString linkDir() const;
    void dirPath(const QString &dirPath);

private:
    QString m_fileName;     // Filename  of the linked file
    QString m_dirPath;      // Directory of the linked file
    QString m_linkName;     // Filename  of the link
    QString m_linkDir;      // Directory of the link
    QString m_url;          // URL
    bool m_isUrl;
};

#endif // LINK_H_
