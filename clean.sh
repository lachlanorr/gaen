#!/bin/sh

# Delete any builds that have been made.
# This is the nuclear option, bootstrap.sh (or cmake manually) must be
# run after this.
# This script won't touch the external expanded tarballs,
# if you need to clean those, do it manually.

BUILD_TYPES=( Debug Release RelWithDebInfo MinSizeRel )
GAEN_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_ROOT="$GAEN_ROOT/build"

for bt in "${BUILD_TYPES[@]}"
do
    BUILD_DIR=$BUILD_ROOT/$bt
    if [ -d "$BUILD_DIR" ]
    then
        echo Cleaning $bt
        rm -rf $BUILD_DIR
    fi
done

rmdir $BUILD_ROOT
