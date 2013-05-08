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

#ifndef DATABASE_H_
#define DATABASE_H_

#include "element.h"
#include <string>
#include <vector>

typedef std::vector<Element>::iterator DatabaseIterator;

class Database {
public:
    Database();
    Database(const std::string &file_path);
    Element &element(const std::string &element_name);
    bool open(const std::string &file_path);
    bool save(const std::string &file_path) const;
    int  findElement(const std::string &element_name) const;
    bool deleteElement(const std::string &element_name);
    bool merge(const std::string &from, const std::string &to);
    DatabaseIterator begin();
    DatabaseIterator end();
    void sort();
    unsigned int getElementCount(const std::string &element_name) const;
    unsigned int nbElements() const;
private:
    static bool sortFunc(Element a, Element b);
    std::vector<Element> m_elements;
};

#endif // DATABASE_H_
