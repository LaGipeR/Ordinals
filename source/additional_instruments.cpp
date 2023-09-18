//
// Created by lagiper on 17.08.23.
//

#include "additional_instruments.h"
#include "constants.h"

#include <iostream>

void update() {
    send_request_to_ord_wallet(OrdWalletCommand::GetBalance, Output::Silent);
}

void init(Menu &main_menu) {
    update();
    read_ordinals_data();

    main_menu.add_element({"exit", "close", "stop"}, ProgramCommand::CloseProgram);
    main_menu.add_element({"show", "buffer"}, ProgramCommand::ShowBuffer);
    main_menu.add_element({"balance", "bal"}, ProgramCommand::Balance);
    main_menu.add_element({"address"}, ProgramCommand::Address);
    main_menu.add_element({"send"}, ProgramCommand::Send);
    main_menu.add_element({"inscribe"}, ProgramCommand::CreateOrdinal);
    main_menu.add_element({"use", "usewallet", "use_wallet"}, ProgramCommand::UseWallet);
    main_menu.add_element({"createwallet", "walletcreate", "create"}, ProgramCommand::CreateOrdWallet);
    main_menu.add_element({"find"}, ProgramCommand::Find);


    std::cout << "Program successfully started\n";
}

void unknown_command_func(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::cout << "This command do nothing\n";
}

std::vector<std::string> split(const std::string &source, std::string delimiter) {
    std::string cur_word;
    std::vector<std::string> result;
    size_t complete_delimiter = 0;

    for (size_t i = 0; i < source.size(); ++i) {
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

    std::cout << "\nProgram successfully finished\n";

    exit(0);
}

void work_with_console(std::string request, const Output &output) {
    if (output == Output::Silent) {
        request += " > " + SILENT_FILENAME;
    }

    system(request.c_str());

    if (output == Output::Silent) {
        system(("rm " + SILENT_FILENAME).c_str());
    }
}
