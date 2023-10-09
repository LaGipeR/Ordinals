#include "menu.h"

#include "additional_instruments.h"
#include "ord_wallet_instruments.h"
#include "bitcoin_instruments.h"

#include <iostream>
#include <climits>
#include <iomanip>
#include <map>

const std::map<ProgramCommand, void (*)(std::vector<std::string>::iterator,
                                        std::vector<std::string>::iterator)> program_command2func = {
        {CloseProgram,    close},
        {Balance,         get_balance},
        {Address,         get_address},
        {Send,            send2address},
        {CreateOrdinal,   create_ordinal},
        {UseWallet,       use_wallet},
        {CreateOrdWallet, create_wallet},
        {Find,            find},
        {Ordinals,        my_ordinals}
};

Menu::Menu(int change_cost, int delete_cost, int insert_cost, int error_limit) : change_cost(change_cost),
                                                                                 delete_cost(delete_cost),
                                                                                 insert_cost(insert_cost),
                                                                                 error_limit(error_limit) {
    this->add_element({"help"}, ProgramCommand::Help, "print this message");
    this->help_token = ProgramCommand::Help;
}

void
Menu::add_element(const std::set<std::string> &names, const ProgramCommand &token, const std::string &description) {
    for (const auto &[list_names, _token, _description]: this->command_list) {
        for (const std::string &name: names) {
            if (list_names.find(name) != list_names.end()) {
                throw std::invalid_argument("Exist command with name '" + name + "'");
            }
        }
    }

    CommandInfo ci{names, token, description};
    this->command_list.push_back(ci);
}

void Menu::print_help_message() {
    std::cout << "You can use next command names:\n";

    for (const auto &[names, _, description]: this->command_list) {
        bool is_first = true;
        std::string command_names;
        for (const std::string &name: names) {
            std::string tmp = (!is_first ? ", " : "") + name;

            if (command_names.size() + tmp.size() > MENU_MAX_COMMAND_NAME_LEN) {
                std::cout << std::setw(MENU_MAX_COMMAND_NAME_LEN) << std::left << command_names << "\n";

                command_names = name;
            } else {
                command_names += tmp;
            }

            is_first = false;
        }

        std::cout << std::setw(MENU_MAX_COMMAND_NAME_LEN) << std::left << command_names << "\t\t" << description
                  << "\n";
    }
    std::cout << std::setw(0);
}

void Menu::execute(const std::string &command_line) {
    std::vector<std::string> command_parts = split(command_line, " ");

    auto [command_name_info, command_token] =
            this->find_best_command_name_len_and_token(command_parts.begin(), command_parts.end());

    if (command_name_info.string_dif_value > this->error_limit) {
        std::cout << "Command could not be recognized\n";
        this->print_help_message();
        return;
    }

    if (command_token == this->help_token) {
        this->print_help_message();
        return;
    }

    program_command2func.at(command_token)(command_name_info.command_name_end, command_parts.end());
}

std::pair<StringDifInfo, ProgramCommand> Menu::find_best_command_name_len_and_token(
        std::vector<std::string>::iterator command_name_and_params_begin,
        std::vector<std::string>::iterator command_name_and_params_end) {
    std::pair<StringDifInfo, ProgramCommand> glob_token_best_res = {{INT_MAX, command_name_and_params_begin},
                                                                    this->help_token};

    for (const auto &[possible_command_names_set, command_token, _]: this->command_list) {
        StringDifInfo cur_token_best_res = calc_best_string_dif_for_set(possible_command_names_set,
                                                                        command_name_and_params_begin,
                                                                        command_name_and_params_end);

        if (cur_token_best_res.better_than(glob_token_best_res.first)) {
            glob_token_best_res = {cur_token_best_res, command_token};
        }
    }

    return glob_token_best_res;
}

StringDifInfo Menu::calc_best_string_dif_for_set(const std::set<std::string> &possible_command_names,
                                                 std::vector<std::string>::iterator command_name_and_params_begin,
                                                 std::vector<std::string>::iterator command_name_and_params_end) {
    StringDifInfo best_string_dif_result_for_set = {INT_MAX, command_name_and_params_begin};

    for (const std::string &name: possible_command_names) {
        StringDifInfo cur_possible_name_string_dif = calc_best_string_dif(name, command_name_and_params_begin,
                                                                          command_name_and_params_end);

        if (cur_possible_name_string_dif.better_than(best_string_dif_result_for_set)) {
            best_string_dif_result_for_set = cur_possible_name_string_dif;
        }
    }

    return best_string_dif_result_for_set;
}

StringDifInfo Menu::calc_best_string_dif(const std::string &possible_command_name,
                                         std::vector<std::string>::iterator command_name_and_params_begin,
                                         std::vector<std::string>::iterator command_name_and_params_end) {

    StringDifInfo best_string_dif = {INT_MAX, command_name_and_params_begin};
    std::string cur_command_name;
    while (command_name_and_params_begin != command_name_and_params_end) {
        if (!cur_command_name.empty()) {
            cur_command_name.push_back(' ');
        }
        cur_command_name += *(command_name_and_params_begin++);

        if (cur_command_name.size() > MENU_MAX_COMMAND_NAME_LEN) {
            break;
        }

        int cur_string_dif_value = calc_string_dif(possible_command_name, cur_command_name);

        if (cur_string_dif_value <= best_string_dif.string_dif_value) {
            best_string_dif = {cur_string_dif_value, command_name_and_params_begin};
        }
    }

    return best_string_dif;
}

int Menu::calc_string_dif(const std::string &possible_command_name, const std::string &command_name) const {
    int dp[possible_command_name.size() + 1][command_name.size() + 1];

    dp[0][0] = 0;
    for (int i = 1; i <= possible_command_name.size(); ++i) {
        dp[i][0] = i * this->insert_cost;
    }
    for (int j = 1; j <= command_name.size(); ++j) {
        dp[0][j] = j * this->delete_cost;
    }

    for (int i = 0; i < possible_command_name.size(); ++i) {
        for (int j = 0; j < command_name.size(); ++j) {
            dp[i + 1][j + 1] = std::min(std::min(
                                                dp[i][j] + this->change_cost * (possible_command_name[i] != command_name[j]),
                                                dp[i][j + 1] + this->delete_cost),
                                        dp[i + 1][j] + this->insert_cost
            );
        }
    }

    return dp[possible_command_name.size()][command_name.size()];
}

bool StringDifInfo::better_than(const StringDifInfo &other) const {
    return this->string_dif_value < other.string_dif_value ||
           (this->string_dif_value == other.string_dif_value && this->command_name_end > other.command_name_end);
}
