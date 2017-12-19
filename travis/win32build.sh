#!/bin/bash

QMAKE=qmake-static
mkdir -p build && mkdir -p assets && cd build && $QMAKE ../ && make -j4 && cd release && cp -Rf *.exe ../../assets/ && cd ../../assets/ && upx *.exe
