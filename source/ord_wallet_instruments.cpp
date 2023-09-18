//
// Created by lagiper on 07.09.23.
//

#include "ord_wallet_instruments.h"
#include "constants.h"
#include "bitcoin_instruments.h"
#include "bitcoinapi/bitcoinapi.h"

#include <iostream>
#include <fstream>

std::string cur_wallet_name = "ord";

void send_request_to_ord_wallet(const std::string &command, const Output &output) {
    std::string request = std::string() +
                          ORD_WALLET_PATH + " " +
                          "-t" + " " +
                          "--bitcoin-rpc-user " + USER + " " +
                          "--bitcoin-rpc-pass " + PASSWORD + " " +
                          "--rpc-url http://" + IP + ":" + PORT + " " +
                          "--wallet " + cur_wallet_name + " " +
                          "--data-dir " + DATABASE_PATH + " " +
                          "wallet" + " " +
                          command + " ";

    work_with_console(request, output);
}

void send_request_to_ord_wallet(const OrdWalletCommand command_name, const Output &output) {
    send_request_to_ord_wallet(ord_wallet_command2string.at(command_name), output);
}

void get_balance(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::GetBalance));
}

void get_address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::GetReceiveAddress));
}

void send2address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "Address to send not input\n";
        return;
    }
    std::string address = *(begin++);

    if (begin == end) {
        std::cout << "Ordinal ID to send not input\n";
        return;
    }
    std::string ordinal_ID = *(begin++);

    send_request_to_ord_wallet(
            ord_wallet_command2string.at(OrdWalletCommand::SendToAddress) + " --fee-rate 1 " + address + " " +
            ordinal_ID);
}

void create_ordinal(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "Filename to create Ordinal not input\n";
        return;
    }

    std::string filename = *(begin++);

    send_request_to_ord_wallet(
            ord_wallet_command2string.at(OrdWalletCommand::CreateNewOrdinal) + " --fee-rate 1 \"" + filename + "\"" +
            " > " + SILENT_FILENAME);

    Value ordinal_data;
    std::fstream f(SILENT_FILENAME);
    f >> ordinal_data;
    std::string ordinal_ID = ordinal_data["inscription"].asString();
    std::string ordinal_address = ordinal_data["reveal"].asString();

    add_ordinal(ordinal_ID, {ordinal_address, 0, 1});

    if (!ordinal_ID.empty()) {
        std::cout << "Create ordinal. Ordinal ID " << ordinal_ID << "\n";
    }
}

void use_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "Wallet name not input\n";
        return;
    }

    std::string wallet_name = *(begin++);

    cur_wallet_name = wallet_name;

    std::cout << "Now use " << wallet_name << " wallet\n";
}

void create_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    send_request_to_ord_wallet(OrdWalletCommand::CreateWallet);
}