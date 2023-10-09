//
// Created by lagiper on 17.08.23.
//

#include "additional_instruments.h"

#include "constants.h"
#include "ord_wallet_instruments.h"
#include "bitcoin_instruments.h"

#include <iostream>
#include <fstream>

Menu init() {
    system(("touch ./" + TMP_FILENAME).c_str());
    system(("touch ./" + TMP_ERROR_FILENAME).c_str());

    update_index();

    read_ordinals_data();
    find_update();

    Menu main_menu(MENU_STRING_DIF_DEFAULT_CHANGE_COST, MENU_STRING_DIF_DEFAULT_DELETE_COST,
                   MENU_STRING_DIF_DEFAULT_INSERT_COST, MENU_STRING_DIF_DEFAULT_ERROR_LIMIT);
    main_menu.add_element({"exit", "close", "stop"}, ProgramCommand::CloseProgram, "exit from program");
    main_menu.add_element({"balance"}, ProgramCommand::Balance, "show balance in current wallet");
    main_menu.add_element({"address"}, ProgramCommand::Address, "generate address to receive satoshi");
    main_menu.add_element({"send"}, ProgramCommand::Send, "send ordinal to address");
    main_menu.add_element({"create"}, ProgramCommand::CreateOrdinal, "create ordinal");
    main_menu.add_element({"use", "use wallet", "wallet use"}, ProgramCommand::UseWallet,
                          "change wallet, name that is used");
    main_menu.add_element({"init", "init wallet", "wallet init"},
                          ProgramCommand::CreateOrdWallet, "create wallet");
    main_menu.add_element({"find"}, ProgramCommand::Find, "find ordinals, that was created before");
    main_menu.add_element({"ordinals", "my ordinals"}, ProgramCommand::Ordinals,
                          "show ordinal IDs, that was created earlier");

    return main_menu;
}

std::vector<std::string> split(const std::string &source, const std::string &delimiter) {
    std::string cur_word;
    std::vector<std::string> result;
    size_t complete_delimiter = 0;

    for (size_t i = 0; i < source.size(); ++i) {
        if (source[i] == '"') {

            if (!cur_word.empty() || complete_delimiter != 0) {
                throw std::invalid_argument("Invalid string in split. Quotes start not from beginning of word");
            }

            size_t begin = ++i;

            while (i < source.size() && source[i] != '"') {
                if (source[i] == '\\') {
                    ++i;
                }
                ++i;
            }

            if (source[i] != '"') {
                throw std::invalid_argument("Invalid string in split. Quotes not closed");
            }

            cur_word = source.substr(begin, i);
        }

        if (source[i] == delimiter[complete_delimiter]) {
            complete_delimiter++;

            if (complete_delimiter == delimiter.size()) {
                complete_delimiter = 0;

                if (!cur_word.empty()) {
                    result.push_back(cur_word);
                    cur_word.clear();
                }
            }
        } else {
            cur_word.append(source.substr(i - complete_delimiter, complete_delimiter + 1));
            complete_delimiter = 0;
        }
    }


    cur_word.append(source.substr(source.size() - complete_delimiter, complete_delimiter));
    if (!cur_word.empty()) {
        result.push_back(cur_word);
    }

    return result;
}

void close(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    write_ordinals_date();

    system(("rm " + TMP_FILENAME).c_str());
    system(("rm " + TMP_ERROR_FILENAME).c_str());

    std::cout << "Program successfully finished\n";
    exit(0);
}

std::pair<std::string, std::string> work_with_console(std::string request) {
    request += " 1> " + TMP_FILENAME;
    request += " 2> " + TMP_ERROR_FILENAME;

    system(request.c_str());

    return {file2string(TMP_FILENAME), file2string(TMP_ERROR_FILENAME)};
}

std::string file2string(const std::string &filename) {
    std::ifstream file(filename);

    std::stringstream stream_file;
    stream_file << file.rdbuf();
    std::string string_file = stream_file.str();

    file.close();

    return string_file;
}

Json::Value string2value(const std::string &source) {
    std::stringstream ss;
    ss << source;

    Json::Value json;
    ss >> json;

    return json;
}

