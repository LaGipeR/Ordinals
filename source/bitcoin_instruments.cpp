//
// Created by lagiper on 17.08.23.
//

#include "bitcoin_instruments.h"

#include "bitcoinapi/bitcoinapi.h"
#include "constants.h"
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
    BitcoinAPI bitcoin_node(USER, PASSWORD, IP, stoi(PORT));

    int last_block_height_exist = bitcoin_node.getblockcount();

    for (; next_block_height <= last_block_height_exist; next_block_height++) {
        std::string block_hash = bitcoin_node.getblockhash(next_block_height);
        std::vector<std::string> txs_ID_in_block = bitcoin_node.getblock(block_hash).tx;

//        int k = 0;
        std::cout << "cur block num = " << next_block_height << " " << txs_ID_in_block.size() << std::endl;

        std::vector<getrawtransaction_t> txs_in_block;
        for (const std::string &tx_ID: txs_ID_in_block) {
            txs_in_block.push_back(bitcoin_node.getrawtransaction(tx_ID, 1));
//            ++k;
        }

        std::map<std::string, getrawtransaction_t> prev_tx_in_vin;
//        std::vector<std::vector<getrawtransaction_t>> prev_tx_in_vin(txs_in_block.size());
//        for (int i = 1; i < txs_in_block.size(); ++i) {
//            for (auto &vin: txs_in_block[i].vin) {
//                prev_tx_in_vin[i].push_back(bitcoin_node.getrawtransaction(vin.txid, 1));
//            }
//        }

        bool ordinal_loc_update = true;
        while (ordinal_loc_update) {
            ordinal_loc_update = false;
            for (int i = 1; i < txs_in_block.size(); ++i) {
                const getrawtransaction_t tx = txs_in_block[i];

                long long sum_in_prev_vin = 0;
                for (int j = 0; j < tx.vin.size(); ++j) {
                    const auto vin = tx.vin[j];
                    for (auto &[_ordinal_ID, loc]: ordinals_location) {
                        if (loc.tx_ID != vin.txid) {
                            continue;
                        }

                        ordinal_loc_update = true;

                        long long pos_ordinal_in_tx = sum_in_prev_vin + loc.pos_in_vout;

                        size_t vout_pos = 0;
                        while (vout_pos < tx.vout.size() &&
                               pos_ordinal_in_tx > (long long) (SAT_IN_BIT * tx.vout[vout_pos].value)) {
                            pos_ordinal_in_tx -= (long long) (SAT_IN_BIT * tx.vout[vout_pos].value);

                            vout_pos++;
                        }

                        if (vout_pos == tx.vout.size()) {
                            // todo case Ordinal go to fee
                            throw std::logic_error("Ordinal go to fee\n");
                        } else {
                            loc = {tx.txid, vout_pos, pos_ordinal_in_tx};
                        }
                    }

                    if (prev_tx_in_vin.find(vin.txid) == prev_tx_in_vin.end()) {
                        prev_tx_in_vin[vin.txid] = bitcoin_node.getrawtransaction(vin.txid, 2);
//                        ++k;
                    }

                    sum_in_prev_vin += (long long) (prev_tx_in_vin[vin.txid].vout[vin.n].value * SAT_IN_BIT);
                }
            }
        }
//        std::cout << k << "\n";
    }

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
    std::cout << ordinal_loc.tx_ID << ":" << ordinal_loc.vout << "\n";
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
        ordinal_data_file << ID << " " << loc.tx_ID << " " << loc.vout << " " << loc.pos_in_vout << "\n";
    }

    ordinal_data_file.close();
}

void add_ordinal(const std::string& ordinal_ID, Location ordinal_location) {
    ordinals_location[ordinal_ID] = std::move(ordinal_location);
}