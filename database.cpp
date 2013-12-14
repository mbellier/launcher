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

/*!
 * \file database.cpp
 * \brief String database
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \copyright This project is released under the GNU Public License.
 */

#include "database.h"
#include <algorithm>
#include <fstream>

using namespace std;


Database::Database(){
}

Database::Database(const string &file_path){
  open(file_path);
}

int Database::findElement(const string &element_name) const {
  for (unsigned int i = 0; i < m_elements.size(); i++) {
    if (m_elements.at(i).name() == element_name){
      return i;
    }
  }
  return -1;
}

Element &Database::element(const std::string &element_name){
  int find = findElement(element_name);
  if (find >= 0) return m_elements.at(find);

  m_elements.push_back(Element(element_name));
  return m_elements.back();
}

bool Database::open(const std::string &file_path){
  string line1, line2, line;

  ifstream myfile (file_path.c_str());
  if (!myfile.is_open()) {
    return false;
  }
  while ( myfile.good() ){
    getline (myfile,line1);
    if (myfile.good()) {
      getline (myfile, line2);
      Element &e = this->element(line1);
      for (int i = 0; i < atoi(line2.c_str()); i++) {
        if (!myfile.good()){
          myfile.close();
          return false;
        }
        getline(myfile, line);
        e.addData(line);
      }
    }

  }
  myfile.close();

  return true;
}

bool Database::save(const std::string &file_path) const{
  ofstream myfile (file_path.c_str());
  if (!myfile.is_open()) {
    return false;
  }

  for (unsigned int i = 0; i < m_elements.size(); i++){
    Element e = m_elements[i];
    myfile << e.name() << endl << e.count() << endl;
    for (unsigned int j = 0; j < e.count(); j++) {
      myfile << e.data(j) << endl;
    }
  }

  myfile.close();
  return true;
}


bool Database::merge(const string &from, const string &to){
  int findFrom = findElement(from);
  if (findFrom < 0) return false;
  element(to).copy((Element)m_elements.at(findFrom));
  deleteElement(from);
  return true;
}

DatabaseIterator Database::begin() {
  return m_elements.begin();
}

DatabaseIterator Database::end(){
  return m_elements.end();
}

void Database::sort(){
  std::sort(m_elements.begin(), m_elements.end(), Database::sortFunc);
}

unsigned int Database::getElementCount(const string &element_name) const {
  int find = findElement(element_name);
  if (find < 0) return 0;

  return m_elements.at(find).count();
}

unsigned int Database::nbElements() const {
  return m_elements.size();
}

bool Database::sortFunc(Element a, Element b){
  return a.count() > b.count();
}


bool Database::deleteElement(const string &element_name){
  int find = findElement(element_name);
  if (find < 0) return false;
  m_elements.erase(m_elements.begin() + find);
  return true;
}
