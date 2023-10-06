//
// Created by lagiper on 07.09.23.
//

#include "ord_wallet_instruments.h"

#include "constants.h"
#include "bitcoin_instruments.h"

#include <iostream>
#include <utility>

const std::map<OrdWalletCommand, std::string> ord_wallet_command2string = {
        {CreateWallet,      "create"},
        {GetBalance,        "balance"},
        {GetReceiveAddress, "receive"},
        {SendToAddress,     "send"},
        {CreateNewOrdinal,  "inscribe"},
};

std::string cur_wallet_name = "ord";

ord_wallet_error::ord_wallet_error(const std::string &m) {
    if (m.find("Couldn't connect to host: Connection refused") != std::string::npos) {
        message = "Couldn't connect to the bitcoin node";
        return;
    }

    if (m.find("Wallet file verification failed. Failed to load database path") != std::string::npos && m.find("Path does not exist.")) {
        message = "Couldn't find wallet with name " + cur_wallet_name + ". Maybe it not created";
        return;
    }

    if (m.find("Wallet file verification failed. Failed to create database path") != std::string::npos && m.find("Database already exists.")) {
        message = "Wallet with name " + cur_wallet_name + " is already created";
        return;
    }

    if (m.find("wallet contains no cardinal utxos") != std::string::npos) {
        message = "In wallet " + cur_wallet_name + " not enough satoshi";
        return;
    }

    if (m.find("No such file or directory") != std::string::npos) {
        message = "File not found";
        return;
    }

    if (m.find("file must have extension") != std::string::npos) {
        message = "File must have extension";
        return;
    }

    if (m.find("Inscription") != std::string::npos && m.find("not found") != std::string::npos) {
        message = "Ordinal not found";
        return;
    }

    if (m.find("invalid value") != std::string::npos && m.find("for '<OUTGOING>': unknown denomination:") != std::string::npos) {
        message = "Invalid Ordinal ID";
        return;
    }

    if (m.find("outgoing satpoint") != std::string::npos && m.find("not in wallet") != std::string::npos) {
        message = "Ordinal is not in wallet " + cur_wallet_name;
        return;
    }

    if (m.find("invalid value") != std::string::npos && m.find("for '<ADDRESS>'") != std::string::npos) {
        message = "Invalid address";
        return;
    }

    message = m;
}

std::pair<std::string, std::string> send_request_to_ord_wallet(const std::string &command) {
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

    return work_with_console(request);
}

std::pair<std::string, std::string> send_request_to_ord_wallet(const OrdWalletCommand &command_name) {
    return send_request_to_ord_wallet(ord_wallet_command2string.at(command_name));
}

void get_balance(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end) {
    auto [balance_answer, balance_error] = send_request_to_ord_wallet(ord_wallet_command2string.at(OrdWalletCommand::GetBalance));

    if (!balance_error.empty()) {
        throw ord_wallet_error(balance_error);
    }

    Json::Value balance_json = string2value(balance_answer);

    std::cout << "Current balance " << balance_json["cardinal"].asString() << " satoshi\n";
}

void get_address(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end) {
    auto [receive_address_answer, receive_address_error] = send_request_to_ord_wallet(
            ord_wallet_command2string.at(OrdWalletCommand::GetReceiveAddress));

    if (!receive_address_error.empty()) {
        throw ord_wallet_error(receive_address_error);
    }

    Json::Value address_json = string2value(receive_address_answer);

    std::cout << "Address to receive " << address_json["address"].asString() << "\n";
}

void send2address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        throw std::invalid_argument("Address not input\nCommand should look like:\nsend <ADDRESS> <ORDINAL_ID>");
    }
    std::string address = *(begin++);

    if (begin == end) {
        throw std::invalid_argument("Ordinal ID not input\nCommand should look like:\nsend <ADDRESS> <ORDINAL_ID>");
    }
    std::string ordinal_ID = *(begin++);

    auto [send_to_address_answer, send_to_address_error] = send_request_to_ord_wallet(
            ord_wallet_command2string.at(OrdWalletCommand::SendToAddress) + " --fee-rate 1 " + address + " " +
            ordinal_ID);

    if (!send_to_address_error.empty()) {
        throw ord_wallet_error(send_to_address_error);
    }

    Json::Value tx_ID_json = string2value(send_to_address_answer);

    std::cout << "Ordinal " << ordinal_ID << " successful send to " << address << "\n";
    std::cout << "Transaction ID: " << tx_ID_json["transaction"] << "\n";
}

void create_ordinal(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        throw std::invalid_argument("Filename to create Ordinal not input\nCommand should look like:\ncreate <FILE>");
    }

    std::string filename = *(begin++);

    auto [create_ordinal_answer, create_ordinal_error] = send_request_to_ord_wallet(
            ord_wallet_command2string.at(OrdWalletCommand::CreateNewOrdinal) + " --fee-rate 1 \"" + filename + "\"");

    if (!create_ordinal_error.empty()) {
        throw ord_wallet_error(create_ordinal_error);
    }

    Json::Value ordinal_data_json = string2value(create_ordinal_answer);

    std::string ordinal_ID = ordinal_data_json["inscription"].asString();
    std::string ordinal_address = ordinal_data_json["reveal"].asString();

    add_ordinal(ordinal_ID, {ordinal_address, 0, 1});

    std::cout << "Successful create new ordinal. Ordinal ID " << ordinal_ID << "\n";
}

void use_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        throw std::invalid_argument("Wallet name not input\nCommand should look like:\nuse <WALLET NAME>");
    }

    std::string wallet_name = *(begin++);

    cur_wallet_name = wallet_name;

    std::cout << "Now use " << wallet_name << " wallet\n";
}

void create_wallet(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end) {
    auto [create_wallet_answer, create_wallet_error] = send_request_to_ord_wallet(OrdWalletCommand::CreateWallet);

    if (!create_wallet_error.empty()) {
        throw ord_wallet_error(create_wallet_error);
    }

    Json::Value wallet_json = string2value(create_wallet_answer);

    std::cout << "Wallet successful created. Mnemonic phrase:\n" << wallet_json["mnemonic"].asString() << "\n";
}

void update_index() {
    std::string request = std::string() +
                          ORD_WALLET_PATH + " " +
                          "-t" + " " +
                          "--bitcoin-rpc-user " + USER + " " +
                          "--bitcoin-rpc-pass " + PASSWORD + " " +
                          "--rpc-url http://" + IP + ":" + PORT + " " +
                          "--wallet " + cur_wallet_name + " " +
                          "--data-dir " + DATABASE_PATH + " " +
                          "index run";

    work_with_console(request);
}
