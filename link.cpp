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

/**
 * \file link.cpp
 * \brief Link object
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \license{This project is released under the GNU Public License.}
 *
 * Contains parsed informations from a link file.
 *
 */

#include "link.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QtWidgets/QFileIconProvider>

Link::Link()
{
}

Link::Link(const QString &link_name,
           const QString &link_dir,
           const QString &url) :
    m_linkName(link_name),
    m_linkDir(link_dir),
    m_url(url),
    m_isUrl(true)
{
}

Link::Link(const QString &link_name,
           const QString &link_dir,
           const QString &dirPath,
           const QString &file_name) :
    m_dirPath(dirPath),
    m_fileName(file_name),
    m_linkName(link_name),
    m_linkDir(link_dir),
    m_isUrl(false)
{

}


QString Link::dirPath() const{
    return m_dirPath;
}
QString Link::fileName() const{
    return m_fileName;
}
QString Link::linkName() const{
    return m_linkName;
}

bool Link::isUrl() const{
    return m_isUrl;
}

QString Link::filePath() const{
    return QString(m_dirPath).append("/").append(m_fileName);
}

QString Link::link() const{
    if (isUrl()){
        return url();
    }
    else{
        return QString("file:///").append(filePath());
    }
}

void Link::dirPath(const QString &dirPath){
    m_dirPath = dirPath;
}

void Link::fileName(const QString &fileName){
    isUrl(false);
    m_fileName = fileName;
}

void Link::linkName(const QString &linkName){
    m_linkName = linkName;
}

QString Link::url() const{
    return m_url;
}

void Link::url(const QString &url){
    isUrl(true);
    m_url = url;
}

void Link::isUrl(const bool &value){
    m_isUrl = value;
}

void Link::linkDir(const QString &linkDir){
    m_linkDir = linkDir;
}

void Link::launch(){
    QDir::setCurrent(dirPath());
    QDesktopServices::openUrl(QUrl(link(), QUrl::TolerantMode));
}

bool Link::openFileLocation(){
    if (isUrl()) return false;
    return QDesktopServices::openUrl(QUrl(QString("file:///").append(dirPath()),
                                          QUrl::TolerantMode));
}

QString Link::linkDir() const {
    return m_linkDir;
}

QString Link::linkExtension() const {
    if (isUrl()) {
        return ".url";
    }
    else {
        return ".lnk";
    }
}


void Link::renameLink(const QString &newLinkName) {
    QDir(linkDir()).rename(linkName().append(linkExtension()),
                           QString(newLinkName).append(linkExtension()));
    linkName(newLinkName);
}


Link *Link::loadSymLink(const QString &filePath, QIcon &icon) {
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


Link *Link::loadURLLink(const QString &filePath, QIcon &icon)
{
  Link *lnk = new Link();
  QFileInfo fileInfo;
  fileInfo.setFile(filePath);
  lnk->linkName(fileInfo.completeBaseName());
  lnk->linkDir(fileInfo.absoluteDir().absolutePath());

  QFile file(filePath);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      if (line.left(4) == "URL=")
      {
        lnk->url(line.mid(4));
      }
      else if (line.left(9) == "IconFile=")
      {
        icon.addFile(line.mid(9));
      }
    }
    file.close();
  }
  else
  {
    delete lnk;
    return NULL;
  }
  return lnk;
}

Link *Link::loadLink(const QString &filePath, QIcon &icon) {
  QString ext = filePath.right(4);
  if (ext == ".lnk") {
    return loadSymLink(filePath, icon);
  }
  else if (ext == ".url") {
    return loadURLLink(filePath, icon);
  }
  return NULL;
}
