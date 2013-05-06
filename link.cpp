/**
 * \file link.cpp
 * \brief Link object
 * \author Maxime B.
 * \version 0.1
 * \date 2013
 * \license{This project is released under the GNU Public License.}
 *
 * Contains parsed informations from a link file.
 *
 */

#include "link.h"

#include <QDesktopServices>
#include <QUrl>
#include <QDir>

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
           const QString &dir_path,
           const QString &file_name) :
    m_dirPath(dir_path),
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

void Link::dirPath(const QString &dir_path){
    m_dirPath = dir_path;
}

void Link::fileName(const QString &file_name){
    isUrl(false);
    m_fileName = file_name;
}

void Link::linkName(const QString &link_name){
    m_linkName = link_name;
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

void Link::linkDir(const QString &link_dir){
    m_linkDir = link_dir;
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


void Link::renameLink(const QString &new_link_name) {
    QDir(linkDir()).rename(linkName().append(linkExtension()),
                           QString(new_link_name).append(linkExtension()));

    linkName(new_link_name);
}
