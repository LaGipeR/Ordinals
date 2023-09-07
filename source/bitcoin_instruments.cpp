//
// Created by lagiper on 17.08.23.
//

#include "bitcoin_instruments.h"

#include "additional_instruments.h"
#include "constants.h"

#include <iostream>

void bitcoin(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::string command;

    Output output = TO_CONSOLE;

    while (begin != end) {
        std::string param = *(begin++);

        if (param == "-c" || param == "--command") {
            if (begin == end) {
                std::cout << "Command not found after '-c' or '--command' parameter\n";
                return;
            }

            command = *(begin++);
        } else if (param == "-b" || param == "--buffer") {
            output = TO_BUFFER;
        } else if (param == "-c" || param == "--console") {
            output = TO_CONSOLE;
        } else if (param == "-bc" || param == "--buffer-and-console") {
            output = TO_BUFFER_AND_CONSOLE;
        }
    }

    request_sender_to_bitcoin_node(command, output);
}

void request_sender_to_bitcoin_node(const std::string &command, const Output &output) {
    std::string request = std::string("") +
                          "curl " +
                          "--user " + USER + ":" + PASSWORD + " " +
                          R"(--data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": ")" + command + R"(", "params": []}' -H 'content-type: text/plain;' )" +
                          IP + ":" + PORT + "/";

    work_with_console(request, output);
}
