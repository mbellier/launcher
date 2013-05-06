/**
 * \file element.cpp
 * \brief Element of a "Database" of strings
 * \author Maxime B.
 * \version 0.1
 * \date 2013
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
