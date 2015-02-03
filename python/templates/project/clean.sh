#!/bin/bash

# Delete any builds that have been made.
# This is the nuclear option, bootstrap.sh (or cmake manually) must be
# run after this.
# This script won't touch the external expanded tarballs,
# if you need to clean those, do it manually.

<<PROJECT_NAME_UPPER>>_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BUILD_ROOT="$<<PROJECT_NAME_UPPER>>_ROOT/build"

if [ -d "$BUILD_ROOT" ]
then
    echo Cleaning $bt
    rm -rf $BUILD_ROOT
fi
