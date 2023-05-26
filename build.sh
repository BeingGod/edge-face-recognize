#!/bin/bash
BUILD_TYPE="Relase"
SCRIPT_HOME=$(cd $(dirname $0); pwd)
LOG_FILE=$SCRIPT_HOME/log/build.log
source $SCRIPT_HOME/scripts/log.sh
set -e

function build()
{
    # build dir
    BUILD_DIR=""
    if [ $BUILD_TYPE="Debug" ]; then
        BUILD_DIR="$SCRIPT_HOME/cmake-build-debug"
    else
        BUILD_DIR="$SCRIPT_HOME/cmake-build-release"
    fi

    # build dependency
    bash $SCRIPT_HOME/scripts/build_qt.sh
    bash $SCRIPT_HOME/scripts/build_cv.sh
    
    # build self
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE \
        -D CMAKE_SYSTEM_PROCESSOR=arm64 \
        -D CMAKE_OSX_ARCHITECTURES=arm64 \
        ..

    make -j7
}
build