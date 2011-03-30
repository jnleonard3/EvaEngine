#!/bin/bash

SYSTEM_TYPE=$(uname -s)
SYSTEM_TYPE=$(echo "$SYSTEM_TYPE" | sed 's/[()\/\._-]//g')
SYSTEM_VERSION=$(uname -r)
SYSTEM_VERSION=$(echo "$SYSTEM_VERSION" | sed 's/[()\/\._-]//g')
CALLING_DIRECTORY=$(pwd)

SYSTEM=$SYSTEM_TYPE-$SYSTEM_VERSION

function printHelp {
	echo "Lol"
}

function build {
	echo "STATUS: Generating '$2' CMake files"
	cd $CALLING_DIRECTORY
	export CMAKE_C_FLAGS="$1"
	export CMAKE_CXX_FLAGS="$1"
	if [ ! -d "build/$SYSTEM/$2" ]
	then
		mkdir -p build/$SYSTEM/$2
	fi
	cd build/$SYSTEM/$2
	cmake ../../../src $3
	echo "STATUS: Building '$2' build"
	make
	echo "STATUS: Done!"
}

function buildDebug {
	build "-g -Wall" debug "-DCMAKE_BUILD_TYPE=Debug $1"
}

function buildRelease {
	build "-Wall" release "-DCMAKE_BUILD_TYPE=Release $1"
}

CLEAN_BUILD=
BUILD_DOXYGEN=
BUILD_TESTS=
BUILD_TYPE=

while getopts "hdtcb:" OPTION
do
	case $OPTION in
		h)
			printHelp
			exit 1
			;;
		d)
			BUILD_DOXYGEN="TRUE"
			;;
		t)
			BUILD_TESTS="TRUE"
			;;
		c)
			CLEAN_BUILD="TRUE"
			;;
		b)
			BUILD_TYPE=$OPTARG
			;;
		?)
			printHelp
			exit 1
			;;
	esac
done

if [ "$CLEAN_BUILD" = "TRUE" ]
then
	cd $CALLING_DIRECTORY
	echo "STATUS: Cleaning build directory"
	rm -Rf build
	echo "STATUS: Done!"
fi

if [ "$BUILD_TESTS" = "TRUE" ]
then
	export EVA_BUILD_TESTS="1"
fi

if [ "$BUILD_TYPE" = "debug" -o "$BUILD_TYPE" = "all" ]
then
	buildDebug
fi

if [ "$BUILD_TYPE" = "release" -o "$BUILD_TYPE" = "all" ]
then
	buildRelease
fi

if [ "$BUILD_DOXYGEN" = "TRUE" -a "$(which doxygen)" != "" ]
then
	cd $CALLING_DIRECTORY
	echo "STATUS: Generating Doxygen documents"
	if [ ! -d "build" ]
	then
		mkdir -p build
	fi
	cd src
	doxygen Doxyfile
	echo "STATUS: Done!"
fi