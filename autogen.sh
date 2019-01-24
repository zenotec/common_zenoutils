#!/bin/sh

sudo apt-get install libtool
sudo apt-get install uuid-dev
sudo apt-get install libnl-3-dev
sudo apt-get install libnl-genl-3-dev
sudo apt-get install libnl-route-3-dev
sudo apt-get install libboost-all-dev
sudo apt-get install libssl-dev
sudo apt-get install libcrypto++-dev
sudo apt-get install libcrypto++-utils
sudo apt-get install libcrypto++6
sudo apt-get install libcrypto++6-dbg

mkdir -p ./build ./config ./m4
touch NEWS README AUTHORS ChangeLog
autoreconf --force --install -I config -I m4

