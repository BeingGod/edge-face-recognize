#!/bin/bash
BUILD_TYPE="Release"
SCRIPT_HOME=$(cd $(dirname $0); pwd)
LOG_FILE=$SCRIPT_HOME/log/build.log
source $SCRIPT_HOME/scripts/log.sh
set -e

function build()
{
    # build dir
    if [ "$BUILD_TYPE" = "Debug" ]; then
        BUILD_DIR="$SCRIPT_HOME/cmake-build-debug"
    else
        BUILD_DIR="$SCRIPT_HOME/cmake-build-release"
    fi

    # build dependency
    bash $SCRIPT_HOME/scripts/build_qt.sh
    
    # build self
    rm -rf $BUILD_DIR
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR

    cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE \
        -D CMAKE_SYSTEM_PROCESSOR=arm64 \
        -D CMAKE_OSX_ARCHITECTURES=arm64 \
        -D CMAKE_INSTALL_PREFIX=$BUILD_DIR \
        -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
        -D CMAKE_VERBOSE_MAKEFILE=1 \
        ..

    make install -j7
}
build
