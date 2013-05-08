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
 * \file element.cpp
 * \brief Element of a "Database" of strings
 * \author Maxime Bellier
 * \version 0.2
 * \date 05.2013
 * \license{This project is released under the GNU Public License.}
 *
 * Named element containging an array of string values.
 *
 */

#include "element.h"

#include <QtDebug>

using namespace std;

Element::Element()
    : m_name(""){

}

Element::Element(const string &element_name)
    : m_name(element_name){

}

unsigned int Element::count() const{
    return m_data.size();
}

string Element::name() const{
    return m_name;
}

std::string Element::data(const unsigned int &index) const{
    return m_data.at(index);
}

bool Element::data(const unsigned int &index, const string &value){
    if (index >= 0 && index < m_data.size()){
        m_data[index] = value;
        return true;
    }
    return false;

}

void Element::addData(const string &value){
    m_data.push_back(value);
}

void Element::copy(const Element &from){
    for (unsigned int i = 0; i < from.count(); i++){
        addData(from.data(i));
    }
}

ElementIterator Element::begin(){
  return m_data.begin();
}

ElementIterator Element::end(){
  return m_data.end();
}
