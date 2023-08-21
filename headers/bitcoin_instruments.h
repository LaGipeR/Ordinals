//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_BITCOIN_INSTRUMENTS_H
#define ORDINALS_BITCOIN_INSTRUMENTS_H

#include <vector>
#include <string>

void start_bitcoin_core(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void stop_bitcoin_core_with_output_option(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, bool with_output = true);

void stop_bitcoin_core(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void info_bitcoin_core(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void bitcoin(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

#endif //ORDINALS_BITCOIN_INSTRUMENTS_H
