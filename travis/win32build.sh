#!/bin/bash

QMAKE=/home/windev/opt/qt32s/bin/qmake
mkdir -p build && mkdir -p assets && cd build && $QMAKE LIBS+=-static ../ && make -j4 && cd release && cp -Rf *.exe ../../assets/ && cd ../../assets/ && upx *.exe
