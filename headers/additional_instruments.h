//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_ADDITIONAL_INSTRUMENTS_H
#define ORDINALS_ADDITIONAL_INSTRUMENTS_H

#include "bitcoin_instruments.h"
#include "ord_wallet_instruments.h"
#include "menu.h"

#include <string>
#include <vector>

void unknown_command_func(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

std::vector<std::string> split(const std::string &source, std::string delimiter);

void close(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void work_with_console(std::string request, const Output &output);

void update();

void init(Menu &main_menu);

#endif //ORDINALS_ADDITIONAL_INSTRUMENTS_H
