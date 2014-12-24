#!/bin/bash

# Run this first on a clean system to prepare Gaen build.
GAEN_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_ROOT=$GAEN_ROOT/build

[ -d "$BUILD_ROOT" ] || mkdir -p "$BUILD_ROOT"

elementIn () {
    local e
    for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
    return 1
}

if [ "$(uname)" == "Darwin" ]; then
    # for OSX, we just use xcode and allow it to manage configs
    cd $BUILD_ROOT

    PLATFORM_TYPES=( osx ios ios-sim )

    if [ -z "$1" ]
    then
        PLATFORM_TYPE=osx
    else
        PLATFORM_TYPE=$1
    fi

    if ! elementIn "$PLATFORM_TYPE" "${PLATFORM_TYPES[@]}"
    then
        echo Invalid platform type \"$PLATFORM_TYPE\", must be one of:
        printf "  %s\n" "${PLATFORM_TYPES[@]}"
        exit 1
    fi

    PLATFORM_DIR="$BUILD_ROOT/${PLATFORM_TYPE}"

    [ -d "$PLATFORM_DIR" ] || mkdir -p "$PLATFORM_DIR"
    cd $PLATFORM_DIR

    if [ "$PLATFORM_TYPE" == "osx" ]
    then
        cmake -G Xcode ${GAEN_ROOT}
    elif [ "$PLATFORM_TYPE" == "ios" ]
    then
        cmake -D CMAKE_TOOLCHAIN_FILE=${GAEN_ROOT}/external/ios-cmake/iOS.cmake -D IOS_ARCH=arm7 -G Xcode ${GAEN_ROOT}
    elif [ "$PLATFORM_TYPE" == "ios-sim" ]
    then
        cmake -D CMAKE_TOOLCHAIN_FILE=${GAEN_ROOT}/external/ios-cmake/iOS.cmake -D IOS_PLATFORM=SIMULATOR -G Xcode ${GAEN_ROOT}
    else
        echo Invalid platform type \"$PLATFORM_TYPE\", must be one of:
        printf "  %s\n" "${PLATFORM_TYPES[@]}"
        exit 1
    fi
else
    # for other unices, we default to makefiles

    BUILD_TYPES=( Debug Release RelWithDebInfo MinSizeRel )
    GAEN_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


    # If you want to override cc options, create a setcc.sh script and
    # place it next to bootstrap.sh.  It will be ignored by git.
    if [ -f ${GAEN_ROOT}/setcc.sh ]
    then
        echo Using custom cc defined in setcc.sh
        source ${GAEN_ROOT}/setcc.sh
    fi


    if [ -z "$1" ]
    then
        BUILD_TYPE=Debug
    else
        BUILD_TYPE=$1
    fi

    if ! elementIn "$BUILD_TYPE" "${BUILD_TYPES[@]}" 
    then
        echo Invalid build type \"$BUILD_TYPE\", must be one of:
        printf "  %s\n" "${BUILD_TYPES[@]}"
        exit 1
    fi

    BUILD_DIR=$BUILD_ROOT/$BUILD_TYPE

    if [ ! -d "$BUILD_DIR" ]
    then
        mkdir -p $BUILD_DIR
    fi

    cd $BUILD_DIR


    # Run cmake
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ${GAEN_ROOT}

    echo
    echo To build, run make within $BUILD_DIR
fi
