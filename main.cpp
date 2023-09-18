#include "additional_instruments.h"
#include "constants.h"

#include "bitcoinapi/bitcoinapi.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
    Menu menu;
    init(menu);

    string command;

    while (true) {
        try {
            getline(cin, command);

            vector<string> components = split(command, " ");

            auto begin = components.begin();
            auto end = components.end();

            program_command2func.at(menu.find_command(begin, end))(begin, end);
        } catch (BitcoinException e) {
            cout << "Something goes wrong in bitcoin node. Try again later\n";
            cout << "Message: " << e.getMessage() << "\n";
        } catch (Json::RuntimeError e) {
            cout << "Something goes wrong in Json parse. Try again later\n";
//            cout << "Message: " << e.what() << "\n";
        } catch (logic_error e) {
            cout << e.what() << "\n";
        }
    }
}