#include <iostream>
#include <vector>

#include "menu.h"
#include "additional_instruments.h"
#include "constants.h"
#include "enums.h"

using namespace std;

int main() {
    string command;
    Menu menu = Menu();
    menu.add_element({"exit", "close"}, ProgramCommand::CloseProgram);
    menu.add_element({"bitcoin"}, ProgramCommand::RequestToBitcoinNode);
    menu.add_element({"show", "buffer"}, ProgramCommand::ShowBuffer);
    menu.add_element({"ord", "ordwallet"}, ProgramCommand::RequestToOrdWallet);
    menu.add_element({"balance", "bal"}, ProgramCommand::Balance);
    menu.add_element({"address"}, ProgramCommand::Address);
    menu.add_element({"send"}, ProgramCommand::Send);
    menu.add_element({"create"}, ProgramCommand::Create);

    cout << "Program successfully started\n";

    while (true) {
        getline(cin, command);

        vector<string> components = split(command, " ");

        auto begin = components.begin();
        auto end = components.end();

        program_command2func.at(menu.find_command(begin, end))(begin, end);
    }
}
