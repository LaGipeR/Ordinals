#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

const std::string IP = "127.0.0.1";
const std::string PORT = "18332";
const std::string USER = "bitcoin";
const std::string PASSWORD = "1";

const std::string TMP_FILENAME = "./data/tmp";
const std::string TMP_ERROR_FILENAME = "./data/tmp_err";

const std::string ORD_WALLET_PATH = R"(/home/lagiper/save\ \(ubuntu\)/Dlabs/bitcoin-25.0/Ordinals/ord/target/release/ord)";
const std::string DATABASE_PATH = R"(/home/lagiper/save\ \(ubuntu\)/Dlabs/bitcoin-25.0/Ordinals/data/)";
const std::string ORDINALS_DATA_FILENAME = "ordinals_data.txt";

const long long SAT_IN_BIT = 100'000'000;

const int MENU_STRING_DIF_DEFAULT_CHANGE_COST = 1;
const int MENU_STRING_DIF_DEFAULT_DELETE_COST = 1;
const int MENU_STRING_DIF_DEFAULT_INSERT_COST = 1;
const int MENU_STRING_DIF_DEFAULT_ERROR_LIMIT = 3;

const int MENU_MAX_COMMAND_NAME_LEN = 20;

const int TRANSACTION_MANAGER_TRANSACTION_NUMBER_LIMIT = (1 << 10);

#endif // CONSTANTS_H