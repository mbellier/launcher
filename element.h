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

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>
#include <vector>

typedef std::vector<std::string>::iterator ElementIterator;

class Element {
public:
    Element();
    Element(const std::string &element_name);
    unsigned int count() const;
    std::string name() const;
    std::string data(const unsigned int &index) const;
    bool data(const unsigned int &index, const std::string &value);
    void addData(const std::string &value);
    void copy(const Element &from);
    ElementIterator begin();
    ElementIterator end();
private:
    std::string m_name;
    std::vector<std::string> m_data;
};

#endif // ELEMENT_H_
