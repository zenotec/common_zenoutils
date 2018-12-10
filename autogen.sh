#!/bin/sh

sudo apt-get install libtool
sudo apt-get install uuid-dev
sudo apt-get install libnl-3-dev
sudo apt-get install libnl-genl-3-dev
sudo apt-get install libnl-route-3-dev
sudo apt-get install libboost-all-dev

mkdir -p ./build ./config ./m4
touch NEWS README AUTHORS ChangeLog
autoreconf --force --install -I config -I m4

