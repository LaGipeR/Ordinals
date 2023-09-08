//
// Created by lagiper on 07.09.23.
//

#include "ord_wallet_instruments.h"

#include "constants.h"

#include <iostream>

void ord(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::string command;
    OrdWalletCommand ord_command;

    Output output = ToConsole;

    while (begin != end) {
        std::string param = *(begin++);

        if (param == "-c" || param == "--command") {
            if (begin == end) {
                std::cout << "Command not found after '-c' or '--command' parameter\n";
                return;
            }

            command = *(begin++);

            if (command == "create") {
                ord_command = OrdWalletCommand::CreateNewWallet;
            } else if (command == "receive") {
                ord_command = OrdWalletCommand::GetReceiveAddress;
            } else if (command == "inscribe") {
                ord_command = OrdWalletCommand::CreateNewOrdinal;
            } else {
                std::cout << "Undefined command";
                return;
            }

        } else if (param == "-b" || param == "--buffer") {
            output = ToBuffer;
        } else if (param == "-c" || param == "--console") {
            output = ToConsole;
        } else if (param == "-bc" || param == "--buffer-and-console") {
            output = ToBufferAndConsole;
        }
    }

    send_request_to_ord_wallet(ord_wallet_command2string.at(ord_command), output);
}

void send_request_to_ord_wallet(const std::string &command, const Output &output = Output::ToConsole) {
    std::string request = std::string() +
                          ORD_WALLET_PATH + " " +
                          "-t" + " " +
                          "--bitcoin-rpc-user " + USER + " " +
                          "--bitcoin-rpc-pass " + PASSWORD + " " +
                          "--rpc-url " + IP + ":" + PORT + " " +
                          "wallet" + " " +
                          command + " ";

    work_with_console(request, output);
}


void get_balance(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::GetBalance));
}

void get_address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::GetReceiveAddress));
}

void send2address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "Address to send not found\n";
        return;
    }

    std::string address = *(begin++);

    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::SendToAddress) + " " + address + "--fee-rate 1");
}

void create_ordinal(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "File to create Ordinal not found\n";
        return;
    }

    std::string filename = *(begin++);

    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::CreateNewOrdinal) + " --fee-rate 1 " + filename);
}