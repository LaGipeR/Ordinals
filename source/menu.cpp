#include <menu.h>
#include <iostream>

Menu::Menu() {};

void Menu::add_element(std::set<std::string> names, void (*func)(std::vector<std::string>::iterator, std::vector<std::string>::iterator)) {
    for (const auto &[list_names, _] : this->list) {
        for (const std::string &name : names) {
            if (list_names.find(name) != list_names.end()) {
                throw std::invalid_argument("Exist command with name '" + name + "'");
            }
        }
    }

    std::pair<std::set<std::string>, void (*)(std::vector<std::string>::iterator, std::vector<std::string>::iterator)> p{names, func};
    this->list.push_back(p);
}

void Menu::find_and_execute(std::vector<std::string>::iterator name_and_components_begin, std::vector<std::string>::iterator name_and_components_end) {
    if (name_and_components_begin == name_and_components_end) {
        std::cout << "Empty input.\n";
        return;
    }

    std::string name = *name_and_components_begin;
    name_and_components_begin++;

    for (const auto &[names, func] : this->list) {
        if (names.find(name) != names.end()) {
            func(name_and_components_begin, name_and_components_end);
            return;
        }
    }

    std::cout << "Not found command with name '" << name << "'\n";
}