//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_BITCOIN_INSTRUMENTS_H
#define ORDINALS_BITCOIN_INSTRUMENTS_H

#include "constants.h"

#include <vector>
#include <string>
#include <map>
#include <bitcoinapi/types.h>
#include <bitcoinapi/bitcoinapi.h>

struct Location {
    std::string tx_ID;
    size_t vout_num;
    long long pos_in_vout;
};

class BitcoinNodeRequestManager {
public:
    BitcoinNodeRequestManager(const std::string &address, int port, const std::string &username,
                              const std::string &password,
                              int transactions_limit = TRANSACTION_MANAGER_TRANSACTION_NUMBER_LIMIT);

    getrawtransaction_t get_tx(const std::string &transaction_ID);

    blockinfo_t get_block(int block_height);

    int last_block_height();

private:
    BitcoinAPI bitcoin_node;
    std::map<std::string, getrawtransaction_t> transactions;

    int transaction_number_limit;

    void check_limits();
};

void find(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void find_update();

long long calc_ordinal_pos_in_tx_vin(BitcoinNodeRequestManager &bitcoin_node_manager, const Location &ordinal_location,
                                     const getrawtransaction_t &transaction_with_ordinal);

Location calc_ordinal_pos_in_tx_vout(const getrawtransaction_t &transaction, long long ordinal_pos_in_tx_vin);

Location calc_ordinal_pos_in_fee(BitcoinNodeRequestManager &bitcoin_node_manager, const blockinfo_t &block,
                                 const getrawtransaction_t &transaction_with_ordinal,
                                 long long ordinal_pos_in_tx_vin);

long long calc_sum_in_tx_vin(BitcoinNodeRequestManager &bitcoin_node_manager, const getrawtransaction_t &transaction);

long long calc_sum_in_tx_vout(const getrawtransaction_t &transaction);

void read_ordinals_data();

void write_ordinals_date();

void add_ordinal(const std::string &ordinal_ID, Location ordinal_location);

#endif //ORDINALS_BITCOIN_INSTRUMENTS_H
