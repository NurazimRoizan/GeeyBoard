#!/bin/bash
# makeme.sh - build this project with docker

# run from the project dir
P=$(realpath -e $0)
PROJDIR="$(dirname ${P})"
cd $PROJDIR

# copy the library so docker can find it
LIBCOPY=${PROJDIR}/components/unphonelibrary
[ -d $LIBCOPY ] || mkdir -p $LIBCOPY
cp -u ../../*.c* ../../*.h ../../CMakeLists.txt $LIBCOPY
