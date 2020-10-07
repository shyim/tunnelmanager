#!/bin/bash

export QMAKE=qmake-static
export QT_SELECT=qt5-i686-w64-mingw32
mkdir -p build && mkdir -p assets && cd build && $QMAKE ../ && make -j4 && cd release && cp -Rf *.exe ../../assets/ && cd ../../assets/
