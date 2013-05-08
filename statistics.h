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

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "database.h"

#include <string>
#include <iostream>

class Statistics {
public:
    Statistics();
    Statistics(const std::string fileName);
    bool open();
    bool open(const std::string fileName);
    void addToken(const std::string appName);
    bool save() const;
    bool save(const std::string fileName) const;
    bool renameElement(const std::string &from, const std::string &to);
    void removeElement(const std::string &elementName);
    unsigned int nbTokens(const std::string &elementName) const;
    unsigned int nbElements() const;
    std::string &fileName();
    DatabaseIterator begin();
    DatabaseIterator end();
    void sort();
private:
    std::string m_fileName;
    std::string tokenData() const;
    Database m_db;
};

#endif // STATISTICS_H
