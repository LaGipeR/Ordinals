#include <iostream>
#include <vector>

#include "menu.h"
#include "additional_instruments.h"
#include "bitcoin_instruments.h"

using namespace std;

int main() {
    string command;
    Menu menu = Menu();
    menu.add_element({"exit", "close"}, close);
    menu.add_element({"bitcoin"}, bitcoin);
    menu.add_element({"start"}, start_bitcoin_core);
    menu.add_element({"show", "buffer"}, show_buffer);

    cout << "Program successfully started\n";

    while (true) {
        getline(cin, command);

        vector<string> components = split(command, " ");

        menu.find_and_execute(components.begin(), components.end());    
    }
}