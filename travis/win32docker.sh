#!/bin/bash

export QT_VERSION=5.6.3
export QT_SELECT=qt5-i686-w64-mingw32
PROJECT_DIR=$(pwd)
DOCKER_IMAGE=syping/qt5-static-mingw:$QT_VERSION
docker pull $DOCKER_IMAGE
docker run --rm \
	-v "${PROJECT_DIR}:/root/tunnelmanager/" \
	$DOCKER_IMAGE \
	/bin/bash -c "cd /root/tunnelmanager; ./travis/win32build.sh"
