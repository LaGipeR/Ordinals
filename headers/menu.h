#ifndef MENU_H
#define MENU_H

#include "enums.h"

#include <vector>
#include <set>
#include <utility>
#include <string>
#include <climits>

struct StringDifInfo {
    int string_dif_value;
    std::vector<std::string>::iterator command_name_end;

    bool better_than(const StringDifInfo &other) const;
};

struct CommandInfo {
    std::set<std::string> names;
    ProgramCommand token;
    std::string description;
};

class Menu {
public:
    Menu(int change_cost, int delete_cost, int insert_cost, int error_limit);

    void add_element(const std::set<std::string> &names, const ProgramCommand &token, const std::string &description);

    std::pair<StringDifInfo, ProgramCommand>
    find_best_command_name_len_and_token(std::vector<std::string>::iterator command_name_and_params_begin,
                                         std::vector<std::string>::iterator command_name_and_params_end);

    void execute(const std::string &command_line);

    void print_help_message();

private:
    int change_cost;
    int delete_cost;
    int insert_cost;

    int error_limit;

    std::vector<CommandInfo> command_list;
    ProgramCommand help_token;

    int calc_string_dif(const std::string &possible_command_name, const std::string &command_name) const;

    StringDifInfo calc_best_string_dif_for_set(const std::set<std::string> &possible_command_names,
                                               std::vector<std::string>::iterator command_name_and_params_begin,
                                               std::vector<std::string>::iterator command_name_and_params_end);

    StringDifInfo calc_best_string_dif(const std::string &possible_command_name,
                                       std::vector<std::string>::iterator command_name_and_params_begin,
                                       std::vector<std::string>::iterator command_name_and_params_end);
};


#endif