//
// Created by lagiper on 17.08.23.
//

#include "additional_instruments.h"
#include "constants.h"

#include <fstream>
#include <iostream>

std::vector<std::string> buffer_list;

std::vector<std::string> split(const std::string &source, std::string delimiter) {
    std::string cur_word;
    std::vector<std::string> result;
    size_t complete_delimiter = 0;

    for (size_t i = 0; i < source.size(); ++i) {
        if (source[i] == delimiter[complete_delimiter]) {
            complete_delimiter++;

            if (complete_delimiter == delimiter.size()) {
                complete_delimiter = 0;

                if (!cur_word.empty()) {
                    result.push_back(cur_word);
                    cur_word.clear();
                }
            }
        } else {
            cur_word.append(source.substr(i - complete_delimiter, complete_delimiter + 1));
            complete_delimiter = 0;
        }
    }


    cur_word.append(source.substr(source.size() - complete_delimiter, complete_delimiter));
    if (!cur_word.empty()) {
        result.push_back(cur_word);
    }

    return result;
}

void close(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin != end && (*begin) == "stop") {
        request_sender("stop", Output::TO_CONSOLE);
    }

    system(("rm " + BUFFER_NAME).c_str());

    std::cout << "\nProgram successfully finished\n";

    exit(0);
}

std::string save_buffer() {
    std::string buffer;

    std::ifstream buffer_file("test.txt");

    std::string line;
    while (getline(buffer_file, line)) {
        buffer += line + "\n";
    }
    buffer_file.close();

    return buffer;
}

void request_sender(const std::string &command, const Output &output) {
    std::string request = std::string("") +
                     "curl " +
                     "--user " + USER + ":" + PASSWORD + " " +
                     R"(--data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": ")" + command + R"(", "params": []}' -H 'content-type: text/plain;' )" +
                     IP + ":" + PORT + "/";

    if (output == Output::TO_BUFFER || output == Output::TO_BUFFER_AND_CONSOLE) {
        request += " > " + BUFFER_NAME;
    }

    system(request.c_str());


    if (output == Output::TO_BUFFER || output == Output::TO_BUFFER_AND_CONSOLE) {
        std::string buffer_el = save_buffer();
        buffer_list.push_back(buffer_el);

        std::cout << "Successful saved in buffer at index " << buffer_list.size() - 1 << "\n";

        if (output == Output::TO_BUFFER_AND_CONSOLE) {
            std::cout << "\n" << buffer_el << "\n";
        }
    }
}


void show_buffer(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    if (begin == end) {
        for (size_t i = 0; i < buffer_list.size(); ++i) {
            std::cout << "index " << i << "\t" << buffer_list[i] << "\n";
        }
    } else {
        while (begin != end) {
            int index = stoi(*(begin++));
            std::cout << "index " << index << "\t" << buffer_list[index] << "\n";
        }
    }
}
