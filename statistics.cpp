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
 * \file statistics.cpp
 * \brief Statistics manager
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
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

Statistics::Statistics(const string fileName) : m_fileName(fileName){
    Statistics();
}

bool Statistics::open(){
    if (m_fileName == ""){
        m_fileName = qApp->applicationDirPath()
                      .append("/statistics.db").toStdString();
    }
    return open(m_fileName);
}

bool Statistics::open(const string fileName){
    return m_db.open(fileName);
}

bool Statistics::save(const string fileName) const{
    return m_db.save(fileName);
}

bool Statistics::save() const{
    return save(m_fileName);
}

void Statistics::addToken(const string appName){
    m_db.element(appName).addData(tokenData());
}


string Statistics::tokenData() const{
    return QDateTime::currentDateTime().toString("yyyy MM dd hh mm").toStdString();
}

bool Statistics::renameElement(const std::string &from, const std::string &to){
    return m_db.merge(from, to);
}

void Statistics::removeElement(const string &elementName){
  m_db.deleteElement(elementName);
}

unsigned int Statistics::nbTokens(const string &elementName) const {
  return m_db.getElementCount(elementName);
}

unsigned int Statistics::nbElements() const{
  return this->m_db.nbElements();
}


string &Statistics::fileName(){
  return m_fileName;
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
