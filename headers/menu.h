#ifndef MENU_H
#define MENU_H

#include <vector>
#include <set>
#include <utility>
#include <string>

#include "additional_instruments.h"

class Menu {
public:
    Menu();

    void add_element(const std::set<std::string> &names, const ProgramCommand &pc);

    ProgramCommand find_command(std::vector<std::string>::iterator &name_and_components_begin, std::vector<std::string>::iterator name_and_components_end);

private:
    std::vector<std::pair< std::set<std::string>, ProgramCommand> > list;
};




#endif