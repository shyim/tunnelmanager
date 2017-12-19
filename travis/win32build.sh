#!/bin/bash

QMAKE=qmake
mkdir -p build && mkdir -p assets && cd build && $QMAKE LIBS+=-static ../ && make -j4 && cd release && cp -Rf *.exe ../../assets/ && cd ../../assets/ && upx *.exe
