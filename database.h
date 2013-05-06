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
