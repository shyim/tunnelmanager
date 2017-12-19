#!/bin/bash

PROJECT_DIR=$(pwd)
QT_VERSION=5.6.3
DOCKER_IMAGE=syping/qt5-static-mingw64:$QT_VERSION
docker pull $DOCKER_IMAGE
docker run --rm \
	-v "${PROJECT_DIR}:/root/tunnelmanager/" \
	$DOCKER_IMAGE \
	/bin/bash -c "cd /root/tunnelmanager; ./travis/win32build.sh"
