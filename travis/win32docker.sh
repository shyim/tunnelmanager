#!/bin/bash

export QT_VERSION=5.15.x
PROJECT_DIR=$(pwd)
DOCKER_IMAGE=syping/qt5-static-llvm-mingw:${QT_VERSION}
docker pull $DOCKER_IMAGE
docker run --rm \
	-v "${PROJECT_DIR}:/root/tunnelmanager/" \
	$DOCKER_IMAGE \
	/bin/bash -c "cd /root/tunnelmanager; ./travis/win32build.sh"
