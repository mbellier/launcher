/**
 * \file statistics.cpp
 * \brief Statistics manager
 * \author Maxime B.
 * \version 0.1
 * \date 2013
 * \license{This project is released under the GNU Public License.}
 *
 * Manages files of statistics.
 *
 */

#include "statistics.h"

#include <QFile>
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QString>
#include <string>
#include <fstream>


using namespace std;

Statistics::Statistics(){
    open();
}

Statistics::Statistics(const string file_name) : m_file_name(file_name){
    Statistics();
}

bool Statistics::open(){
    if (m_file_name == ""){
        m_file_name = qApp->applicationDirPath()
                      .append("/statistics.db").toStdString();
    }
    return open(m_file_name);
}

bool Statistics::open(const string file_name){
    return m_db.open(file_name);
}

bool Statistics::save(const string file_name) const{
    return m_db.save(file_name);
}

bool Statistics::save() const{
    return save(m_file_name);
}

void Statistics::addToken(const string app_name){
    m_db.element(app_name).addData(tokenData());
}


string Statistics::tokenData() const{
    return QDateTime::currentDateTime().toString("yyyy MM dd hh mm").toStdString();
}

bool Statistics::renameElement(const std::string &from, const std::string &to){
    return m_db.merge(from, to);
}

void Statistics::removeElement(const string &element_name){
  m_db.deleteElement(element_name);
}

unsigned int Statistics::nbTokens(const string &element_name) const {
  return m_db.getElementCount(element_name);
}

unsigned int Statistics::nbElements() const{
  return this->m_db.nbElements();
}


string &Statistics::fileName(){
  return m_file_name;
}

DatabaseIterator Statistics::begin(){
  return m_db.begin();
}

DatabaseIterator Statistics::end(){
  return m_db.end();
}

void Statistics::sort(){
  m_db.sort();
}
