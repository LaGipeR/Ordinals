#ifndef MENU_H
#define MENU_H

#include <vector>
#include <set>
#include <utility>
#include <string>

class Menu {
public:
    Menu();

    void add_element(std::set<std::string> names, void (*func)(std::vector<std::string>::iterator, std::vector<std::string>::iterator));

    void find_and_execute(std::vector<std::string>::iterator name_and_components_begin, std::vector<std::string>::iterator name_and_components_end);

private:
    std::vector< std::pair< std::set<std::string>, void(*)(std::vector<std::string>::iterator, std::vector<std::string>::iterator)> > list;
};




#endif