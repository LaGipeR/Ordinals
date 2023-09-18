//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_BITCOIN_INSTRUMENTS_H
#define ORDINALS_BITCOIN_INSTRUMENTS_H

#include <vector>
#include <string>

struct Location {
    std::string tx_ID;
    size_t vout;
    long long pos_in_vout;
};

void find(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void find_update();

void read_ordinals_data();

void write_ordinals_date();

void add_ordinal(const std::string& ordinal_ID, Location ordinal_location);

#endif //ORDINALS_BITCOIN_INSTRUMENTS_H
