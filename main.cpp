#include "additional_instruments.h"
#include "menu.h"
#include "bitcoinapi/bitcoinapi.h"
#include "ord_wallet_instruments.h"

#include <iostream>
#include <string>

int main() {
    Menu menu = init();

    std::cout << "Program started\n";

    std::string command;
    while (true) {
        try {
            getline(std::cin, command);
            menu.execute(command);

        } catch (BitcoinException &e) {
            std::cout << "Something goes wrong in bitcoin node. Try again later\n";
            std::cout << "Message: " << e.getMessage() << "\n";
        } catch (Json::RuntimeError &e) {
            std::cout << "Something goes wrong in Json parse. Try again later\n";
        } catch (std::logic_error &e) {
            std::cout << "ERROR:\n" << e.what() << "\n";
        } catch (ord_wallet_error &e) {
            std::cout << "ERROR:\n" << e.message << "\n";
        }
    }
}