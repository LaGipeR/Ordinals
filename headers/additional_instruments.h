//
// Created by lagiper on 17.08.23.
//

#ifndef ORDINALS_ADDITIONAL_INSTRUMENTS_H
#define ORDINALS_ADDITIONAL_INSTRUMENTS_H

#include "menu.h"

#include "bitcoinapi/types.h"

#include <string>
#include <vector>

std::vector<std::string> split(const std::string &source, const std::string &delimiter);

void close(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

Menu init();

std::pair<std::string, std::string> work_with_console(std::string request);

std::string file2string(const std::string &filename);

Json::Value string2value(const std::string &source);

#endif //ORDINALS_ADDITIONAL_INSTRUMENTS_H
