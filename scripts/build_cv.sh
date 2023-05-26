#!/bin/bash
SCRIPT_HOME=$(cd $(dirname $0); pwd)
LOG_FILE=$SCRIPT_HOME/../log/build.log
source $SCRIPT_HOME/log.sh
set -e

function build_cv()
{
    THIRD_PARTY_DIR="$SCRIPT_HOME/../third_party"

    if [ -d "$THIRD_PARTY_DIR/opencv-release" ]; then
        log "opencv is already build! skip..." WARN
        return
    fi

    cmake --version | head -1

    CV_DIR="$THIRD_PARTY_DIR/opencv"
    CV_TMP="$SCRIPT_HOME/../cmake-build-release/build_cv"
    CV_DST="$THIRD_PARTY_DIR/opencv-release"

    log "CV_DIR=${CV_DIR}" WARN
    log "CV_TMP=${CV_TMP}" WARN
    log "CV_DST=${CV_DST}" WARN

    rm -rf ${CV_TMP}
    mkdir -p ${CV_TMP}
    cd ${CV_TMP}

    cmake -D CMAKE_BUILD_TYPE=Release \
        -D BUILD_ZLIB=OFF \
        -D CMAKE_INSTALL_PREFIX=$CV_DST \
        -D CMAKE_SYSTEM_PROCESSOR=arm64 \
        -D CMAKE_OSX_ARCHITECTURES=arm64 \
        -D CMAKE_BUILD_TYPE=Release \
        -D BUILD_EXAMPLES=OFF \
        -D BUILD_TESTS=OFF \
        -D BUILD_PERF_TESTS=OFF \
        "$CV_DIR"

    make install -j7
}
build_cv
