#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "enums.h"
#include "additional_instruments.h"
#include "ord_wallet_instruments.h"

#include <map>

const std::string IP = "127.0.0.1";
const std::string PORT = "18332";
const std::string USER = "bitcoin";
const std::string PASSWORD = "1";
const std::string SILENT_FILENAME = "silent";
const std::string ORD_WALLET_PATH = R"(~/save\ \(ubuntu\)/Dlabs/bitcoin-25.0/Ordinals/ordwallet/ord)";
const std::string DATABASE_PATH = R"(/home/lagiper/save\ \(ubuntu\)/Dlabs/bitcoin-25.0/Ordinals/data/)";
const std::string ORDINALS_DATA_FILENAME = "ordinals_data.txt";
const long long SAT_IN_BIT = 100'000'000;

const std::map<ProgramCommand, void (*)(std::vector<std::string>::iterator,
                                        std::vector<std::string>::iterator)> program_command2func = {
        {UnknownCommand,  unknown_command_func},
        {CloseProgram,    close},
        {Balance,         get_balance},
        {Address,         get_address},
        {Send,            send2address},
        {CreateOrdinal,   create_ordinal},
        {UseWallet,       use_wallet},
        {CreateOrdWallet, create_wallet},
        {Find,            find},
};

const std::map<OrdWalletCommand, std::string> ord_wallet_command2string = {
        {CreateWallet,      "create"},
        {GetBalance,        "balance"},
        {GetReceiveAddress, "receive"},
        {SendToAddress,     "send"},
        {CreateNewOrdinal,  "inscribe"},
};

#endif // CONSTANTS_H