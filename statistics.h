#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "database.h"

#include <string>
#include <iostream>

class Statistics {
public:
    Statistics();
    Statistics(const std::string file_name);
    bool open();
    bool open(const std::string file_name);
    void addToken(const std::string app_name);
    bool save() const;
    bool save(const std::string file_name) const;
    bool renameElement(const std::string &from, const std::string &to);
    void removeElement(const std::string &element_name);
    unsigned int nbTokens(const std::string &element_name) const;
    unsigned int nbElements() const;
    std::string &fileName();
    DatabaseIterator begin();
    DatabaseIterator end();
    void sort();
private:
    std::string m_file_name;
    std::string tokenData() const;
    Database m_db;
};

#endif // STATISTICS_H
