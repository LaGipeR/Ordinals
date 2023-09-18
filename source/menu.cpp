#include "menu.h"

#include <iostream>

Menu::Menu() {};

void Menu::add_element(const std::set<std::string> &names, const ProgramCommand &pc) {
    for (const auto &[list_names, _]: this->list) {
        for (const std::string &name: names) {
            if (list_names.find(name) != list_names.end()) {
                throw std::invalid_argument("Exist command with name '" + name + "'");
            }
        }
    }

    std::pair<std::set<std::string>, ProgramCommand> p{names, pc};
    this->list.push_back(p);
}

ProgramCommand Menu::find_command(std::vector<std::string>::iterator &name_and_components_begin,
                                  std::vector<std::string>::iterator name_and_components_end) {
    if (name_and_components_begin == name_and_components_end) {
        std::cout << "Empty input.\n";
        return ProgramCommand::UnknownCommand;
    }

    std::string name = *name_and_components_begin;
    name_and_components_begin++;

    for (const auto &[names, pc]: this->list) {
        if (names.find(name) != names.end()) {
            return pc;
        }
    }

    std::cout << "Not found command with name '" << name << "'\n";
    return ProgramCommand::UnknownCommand;
}