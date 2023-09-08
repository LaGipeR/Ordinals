//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_ADDITIONAL_INSTRUMENTS_H
#define ORDINALS_ADDITIONAL_INSTRUMENTS_H

#include <string>
#include <vector>
#include <map>

#include "bitcoin_instruments.h"
#include "ord_wallet_instruments.h"

void unknown_command_func(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

std::vector<std::string> split(const std::string &source, std::string delimiter);

void close(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

std::string safe_buffer();

void pwd(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void show_buffer(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void work_with_console(std::string request, const Output &output);

#endif //ORDINALS_ADDITIONAL_INSTRUMENTS_H
