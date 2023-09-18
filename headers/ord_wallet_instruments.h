//
// Created by lagiper on 07.09.23.
//

#ifndef ORDINALS_ORD_WALLET_INSTRUMENTS_H
#define ORDINALS_ORD_WALLET_INSTRUMENTS_H

#include "additional_instruments.h"
#include "enums.h"

#include <string>
#include <vector>

void send_request_to_ord_wallet(const std::string &command, const Output &output = ToConsole);

void send_request_to_ord_wallet(const OrdWalletCommand command_name, const Output &output = ToConsole);

void get_balance(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void get_address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void send2address(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void create_ordinal(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void use_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void create_wallet(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

#endif //ORDINALS_ORD_WALLET_INSTRUMENTS_H
