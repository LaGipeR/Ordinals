//
// Created by lagiper on 07.09.23.
//

#include "ord_wallet_instruments.h"

#include "constants.h"

#include <iostream>

void ord(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::string command;
    OrdWalletCommand ord_command;

    Output output = TO_CONSOLE;

    while (begin != end) {
        std::string param = *(begin++);

        if (param == "-c" || param == "--command") {
            if (begin == end) {
                std::cout << "Command not found after '-c' or '--command' parameter\n";
                return;
            }

            command = *(begin++);

            if (command == "create") {
                ord_command = OrdWalletCommand::CREATE_NEW_WALLET;
            } else if (command == "receive") {
                ord_command = OrdWalletCommand::GET_RECEIVE_ADDRESS;
            } else if (command == "inscribe") {
                ord_command = OrdWalletCommand::CREATE_NEW_ORDINAL;
            } else {
                std::cout << "Undefined command";
                return;
            }

        } else if (param == "-b" || param == "--buffer") {
            output = TO_BUFFER;
        } else if (param == "-c" || param == "--console") {
            output = TO_CONSOLE;
        } else if (param == "-bc" || param == "--buffer-and-console") {
            output = TO_BUFFER_AND_CONSOLE;
        }
    }

    request_sender_to_ord_wallet(ord_command, output);
}

void request_sender_to_ord_wallet(const OrdWalletCommand &command, const Output &output) {
    std::string request = std::string() +
                          ORD_WALLET_PATH + " " +
                          "-t" + " " +
                          "--bitcoin-rpc-user " + USER + " " +
                          "--bitcoin-rpc-pass " + PASSWORD + " " +
                          "--rpc-url " + IP + ":" + PORT + " " +
                          "wallet " +
                          ord_wallet_command2string.at(command) + " ";

    work_with_console(request, output);
}