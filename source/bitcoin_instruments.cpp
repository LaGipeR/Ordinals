//
// Created by lagiper on 17.08.23.
//

#include "bitcoin_instruments.h"

#include "additional_instruments.h"
#include "constants.h"

#include <iostream>

void bitcoin(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    std::string command;

    Output output = ToConsole;

    while (begin != end) {
        std::string param = *(begin++);

        if (param == "-c" || param == "--command") {
            if (begin == end) {
                std::cout << "Command not found after '-c' or '--command' parameter\n";
                return;
            }

            command = *(begin++);
        } else if (param == "-b" || param == "--buffer") {
            output = ToBuffer;
        } else if (param == "-c" || param == "--console") {
            output = ToConsole;
        } else if (param == "-bc" || param == "--buffer-and-console") {
            output = ToBufferAndConsole;
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
