# Ordinals

# Usage
clone git reposstory
```sh
git clone git@github.com:LaGipeR/Ordinals.git
```

Before start need to build [bitcoin-api-cpp](https://github.com/minium/bitcoin-api-cpp) library

Try this to build bitcoin-api-cpp
```sh
cd bitcoin-api-cpp
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig
cd ../
```
If some problem appeared read more [here](https://github.com/minium/bitcoin-api-cpp)

Also, need to build [ord](https://github.com/ordinals/ord)

Try this to build ord
```sh
git clone git@github.com:ordinals/ord.git
cd ord
cargo build --release
cd ../
```
If some problem appeared read more [here](https://github.com/ordinals/ord)

# Launch
```sh
sh run.sh
```
