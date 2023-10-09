g++ -w -I ./headers -O2 main.cpp source/additional_instruments.cpp source/ord_wallet_instruments.cpp source/bitcoin_instruments.cpp source/menu.cpp -o Ordinals -lbitcoinapi -ljsoncpp
./Ordinals