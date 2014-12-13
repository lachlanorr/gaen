#!/bin/bash

# Run this first on a clean system to prepare Gaen build.

BUILD_TYPES=( Debug Release RelWithDebInfo MinSizeRel )
GAEN_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


# If you want to override cc options, create a setcc.sh script and
# place it next to bootstrap.sh.  It will be ignored by git.
if [ -f ${GAEN_ROOT}/setcc.sh ]
then
    echo Using custom cc defined in setcc.sh
    source ${GAEN_ROOT}/setcc.sh
fi


elementIn () {
  local e
  for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
  return 1
}

abspath () {
    if [[ -d "$1" ]]
    then
        pushd "$1" >/dev/null
        pwd
        popd >/dev/null
    elif [[ -e $1 ]]
    then
        pushd "$(dirname "$1")" >/dev/null
        echo "$(pwd)/$(basename "$1")"
        popd >/dev/null
    else
        echo "$1" does not exist! >&2
        return 127
    fi
}


export GAEN_ROOT=$(abspath "$GAEN_ROOT")

BUILD_ROOT=$GAEN_ROOT/build
[ -d "$BUILD_ROOT" ] || mkdir -p "$BUILD_ROOT"

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
