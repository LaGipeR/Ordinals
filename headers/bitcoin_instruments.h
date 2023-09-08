//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_BITCOIN_INSTRUMENTS_H
#define ORDINALS_BITCOIN_INSTRUMENTS_H

#include <vector>
#include <string>

#include "additional_instruments.h"
#include "enums.h"

void bitcoin(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

void request_sender_to_bitcoin_node(const std::string &command, const Output &output);

#endif //ORDINALS_BITCOIN_INSTRUMENTS_H
