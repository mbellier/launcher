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
