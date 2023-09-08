#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "enums.h"

const std::string IP = "http://127.0.0.1";
const std::string PORT = "18332";
const std::string USER = "bitcoin";
const std::string PASSWORD = "1";
const std::string BUFFER_NAME = "buffer";
const std::string ORD_WALLET_PATH = "../ordwallet/ord";

const std::map<ProgramCommand, void(*)(std::vector<std::string>::iterator, std::vector<std::string>::iterator)> program_command2func = {
        {UnknownCommand, unknown_command_func},
        {CloseProgram, close},
        {RequestToBitcoinNode, bitcoin},
        {RequestToOrdWallet, ord},
        {ShowBuffer, show_buffer},
        {Balance, get_balance},
        {Address, get_address},
        {Send, send2address},
        {Create, create_ordinal},
};

const std::map<OrdWalletCommand, std::string> ord_wallet_command2string = {
        {CreateNewWallet, "create"},
        {GetBalance, "balance"},
        {GetReceiveAddress, "receive"},
        {SendToAddress, "send"},
        {CreateNewOrdinal, "inscribe"},
};

#endif // CONSTANTS_H