//
// Created by lagiper on 07.09.23.
//

#ifndef ORDINALS_ORD_WALLET_INSTRUMENTS_H
#define ORDINALS_ORD_WALLET_INSTRUMENTS_H

#include <string>
#include <map>

#include "additional_instruments.h"

enum OrdWalletCommand {
    CREATE_NEW_WALLET,
    GET_RECEIVE_ADDRESS,
    CREATE_NEW_ORDINAL,
};

const std::map<OrdWalletCommand, std::string> ord_wallet_command2string = {
        {CREATE_NEW_WALLET, "create"},
        {GET_RECEIVE_ADDRESS, "receive"},
        {CREATE_NEW_ORDINAL, "inscribe"}
};

void ord(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void request_sender_to_ord_wallet(const OrdWalletCommand &command, const Output &output);

#endif //ORDINALS_ORD_WALLET_INSTRUMENTS_H
