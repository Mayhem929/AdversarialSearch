#!/bin/bash

# Install SFML, if not Ubuntu, comment next line & check https://www.sfml-dev.org/tutorials/2.5/start-linux.php
sudo apt-get install libsfml-dev

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8
cd ..

./build/ParchisGame