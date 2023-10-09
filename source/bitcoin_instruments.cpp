//
// Created by lagiper on 17.08.23.
//

#include "bitcoin_instruments.h"

#include "additional_instruments.h"

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <numeric>
#include <utility>

int next_block_height = 2'475'000;
std::map<std::string, Location> ordinals_location;

void find_update() {
    BitcoinNodeRequestManager bitcoin_node_manager = BitcoinNodeRequestManager(IP, stoi(PORT), USER, PASSWORD);

    int last_block_height_exist = bitcoin_node_manager.last_block_height();

    for (; next_block_height <= last_block_height_exist; next_block_height++) {
        const auto block = bitcoin_node_manager.get_block(next_block_height);
        std::vector<std::string> txs_ID_in_cur_block = block.tx;

        std::cout << "Current block height analise " << next_block_height << "\r";
        std::cout.flush();

        for (int i = 1; i < txs_ID_in_cur_block.size(); ++i) {
            const getrawtransaction_t tx = bitcoin_node_manager.get_tx(txs_ID_in_cur_block[i]);

            for (const auto &vin: tx.vin) {
                for (auto &[_ordinal_ID, loc]: ordinals_location) {
                    if (!(loc.tx_ID == vin.txid && loc.vout_num == vin.n)) {
                        continue;
                    }

                    std::cout << "\n" << next_block_height << " " << loc.tx_ID << " " << loc.vout_num << " "
                              << loc.pos_in_vout << "\n";

                    long long ordinal_pos_in_tx_vin = calc_ordinal_pos_in_tx_vin(bitcoin_node_manager, loc, tx);

                    if (ordinal_pos_in_tx_vin <= calc_sum_in_tx_vout(tx)) {
                        loc = calc_ordinal_pos_in_tx_vout(tx, ordinal_pos_in_tx_vin);
                    } else {
                        loc = calc_ordinal_pos_in_fee(bitcoin_node_manager, block, tx, ordinal_pos_in_tx_vin);
                    }
                }
            }
        }
    }

    std::cout << std::endl;
}

long long
calc_ordinal_pos_in_tx_vin(BitcoinNodeRequestManager &bitcoin_node_manager, const Location &ordinal_location,
                           const getrawtransaction_t &transaction_with_ordinal) {
    long long pos = 0;

    for (const auto &vin: transaction_with_ordinal.vin) {
        if (vin.txid == ordinal_location.tx_ID && vin.n == ordinal_location.vout_num) {
            return pos + ordinal_location.pos_in_vout;
        }

        pos += static_cast<long long>(bitcoin_node_manager.get_tx(vin.txid).vout[vin.n].value * SAT_IN_BIT);
    }

    throw std::logic_error("ordinal not found when calculate pos in tx");
}

Location calc_ordinal_pos_in_tx_vout(const getrawtransaction_t &transaction, long long ordinal_pos_in_tx_vin) {
    for (size_t i = 0; i < transaction.vout.size(); ++i) {
        auto sat_in_cur_tx_vout = static_cast<long long>(transaction.vout[i].value * SAT_IN_BIT);
        if (ordinal_pos_in_tx_vin <= sat_in_cur_tx_vout) {
            return {transaction.txid, i, ordinal_pos_in_tx_vin};
        }

        ordinal_pos_in_tx_vin -= sat_in_cur_tx_vout;
    }

    throw std::logic_error("ordinal go to fee, but try to calculate in vout position");
}

Location calc_ordinal_pos_in_fee(BitcoinNodeRequestManager &bitcoin_node_manager, const blockinfo_t &block,
                                 const getrawtransaction_t &transaction_with_ordinal,
                                 long long ordinal_pos_in_tx_vin) {

    long long total_fee = 0;
    long long ordinal_pos_in_fee = 0;
    bool is_find = false;
    for (size_t i = 1; i < block.tx.size(); ++i) {
        auto tx = bitcoin_node_manager.get_tx(block.tx[i]);

        long long vin_sum = calc_sum_in_tx_vin(bitcoin_node_manager, tx);
        long long vout_sum = calc_sum_in_tx_vout(tx);
        long long fee = vin_sum - vout_sum;
        total_fee += fee;

        if (tx.txid == transaction_with_ordinal.txid) {
            ordinal_pos_in_fee += ordinal_pos_in_tx_vin - vout_sum;
            is_find = true;
        } else {
            if (!is_find) {
                ordinal_pos_in_fee += fee;
            }
        }
    }

    if (!is_find) {
        throw std::logic_error("Ordinal not found when calculate position in fee");
    }

    getrawtransaction_t coin_base = bitcoin_node_manager.get_tx(block.tx.front());

    long long subsidy = calc_sum_in_tx_vout(coin_base) - total_fee;
    long long ordinal_pos_in_coin_base = subsidy + ordinal_pos_in_fee;

    return calc_ordinal_pos_in_tx_vout(coin_base, ordinal_pos_in_coin_base);
}

long long calc_sum_in_tx_vin(BitcoinNodeRequestManager &bitcoin_node_manager, const getrawtransaction_t &transaction) {
    long long sum = 0;

    for (const auto &vin: transaction.vin) {
        sum += static_cast<long long>(SAT_IN_BIT * bitcoin_node_manager.get_tx(vin.txid).vout[vin.n].value);
    }

    return sum;
}

long long calc_sum_in_tx_vout(const getrawtransaction_t &transaction) {
    long long sum = 0;

    for (const auto &vout: transaction.vout) {
        sum += static_cast<long long>(SAT_IN_BIT * vout.value);
    }

    return sum;
}

void find(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        std::cout << "Ordinal ID not input\n";
        return;
    }

    std::string ordinal_ID = *(begin++);

    if (ordinals_location.find(ordinal_ID) == ordinals_location.end()) {
        std::cout << "Ordinal " << ordinal_ID << " was not inscribe\n";
        return;
    }

    find_update();

    Location ordinal_loc = ordinals_location.at(ordinal_ID);

    std::cout << "Ordinal found in transaction\n";
    std::cout << ordinal_loc.tx_ID << ":" << ordinal_loc.vout_num << "\n";
    std::cout << "Position ordinal in transaction out " << ordinal_loc.pos_in_vout << "\n";
}

void read_ordinals_data() {
    std::vector<std::string> filename_parts = split((DATABASE_PATH + ORDINALS_DATA_FILENAME), "\\");
    std::string filename = std::accumulate(filename_parts.begin(), filename_parts.end(), std::string(""));

    std::fstream ordinal_data_file(filename);

    std::string line;

    getline(ordinal_data_file, line);

    next_block_height = stoi(line);

    while (getline(ordinal_data_file, line)) {
        std::vector<std::string> ordinal_loc_data = split(line, " ");

        ordinals_location[ordinal_loc_data[0]] = {
                ordinal_loc_data[1],
                static_cast<size_t>(stoi(ordinal_loc_data[2])),
                stoll(ordinal_loc_data[3]),
        };
    }
}

void write_ordinals_date() {
    std::vector<std::string> filename_parts = split((DATABASE_PATH + ORDINALS_DATA_FILENAME), "\\");
    std::string filename = std::accumulate(filename_parts.begin(), filename_parts.end(), std::string(""));

    std::ofstream ordinal_data_file(filename);

    ordinal_data_file << next_block_height << "\n";

    for (const auto &[ID, loc]: ordinals_location) {
        ordinal_data_file << ID << " " << loc.tx_ID << " " << loc.vout_num << " " << loc.pos_in_vout << "\n";
    }

    ordinal_data_file.close();
}

void add_ordinal(const std::string &ordinal_ID, Location ordinal_location) {
    ordinals_location[ordinal_ID] = std::move(ordinal_location);
}

BitcoinNodeRequestManager::BitcoinNodeRequestManager(const std::string &address, int port, const std::string &username,
                                                     const std::string &password, int transactions_limit)
        : bitcoin_node(username,
                       password,
                       address,
                       port),
          transaction_number_limit(
                  transactions_limit) {}

getrawtransaction_t BitcoinNodeRequestManager::get_tx(const std::string &transaction_ID) {
    this->check_limits();

    auto tx_it = this->transactions.find(transaction_ID);

    if (tx_it == this->transactions.end()) {
        auto tx = this->bitcoin_node.getrawtransaction(transaction_ID, 2);
        return this->transactions[transaction_ID] = tx;
    } else {
        return tx_it->second;
    }
}

void BitcoinNodeRequestManager::check_limits() {
    if (this->transactions.size() > this->transaction_number_limit) {
        this->transactions.clear();
    }
}

blockinfo_t BitcoinNodeRequestManager::get_block(int block_height) {
    return this->bitcoin_node.getblock(this->bitcoin_node.getblockhash(block_height));
}

int BitcoinNodeRequestManager::last_block_height() {
    return this->bitcoin_node.getblockcount();
}

void my_ordinals(std::vector<std::string>::iterator _begin, std::vector<std::string>::iterator _end) {
    for (const auto &[ordinal_ID, _ordinal_loc]: ordinals_location) {
        std::cout << ordinal_ID << "\n";
    }
}