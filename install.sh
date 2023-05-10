#!/bin/bash

BUILD_DOC=OFF # ON or OFF

# Install SFML, if not Ubuntu, comment next line & check https://www.sfml-dev.org/tutorials/2.5/start-linux.php
sudo apt-get -y install libsfml-dev
# For doxygen generation
if [ "$BUILD_DOC" = "ON" ]; then
    sudo apt-get -y install doxygen graphviz
fi

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOC=$BUILD_DOC .. 
make -j8
cd ..

./build/ParchisGame