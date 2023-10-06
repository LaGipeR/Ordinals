//
// Created by lagiper on 07.09.23.
//

#ifndef ORDINALS_ORD_WALLET_INSTRUMENTS_H
#define ORDINALS_ORD_WALLET_INSTRUMENTS_H

#include "additional_instruments.h"
#include "enums.h"
#include "bitcoinapi/bitcoinapi.h"

#include <string>
#include <vector>
#include <exception>

struct ord_wallet_error: public std::exception {
    std::string message;

    explicit ord_wallet_error(const std::string &m);
};

std::pair<std::string, std::string> send_request_to_ord_wallet(const std::string &command);

std::pair<std::string, std::string> send_request_to_ord_wallet(const OrdWalletCommand &command_name);

void get_balance(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end);

void get_address(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end);

void send2address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void create_ordinal(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void use_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void create_wallet(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end);

void update_index();

#endif //ORDINALS_ORD_WALLET_INSTRUMENTS_H
