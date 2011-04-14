#!/bin/bash

SYSTEM_TYPE=$(uname -s)
SYSTEM_TYPE=$(echo "$SYSTEM_TYPE" | sed 's/[()\/\._-]//g')
SYSTEM_VERSION=$(uname -r)
SYSTEM_VERSION=$(echo "$SYSTEM_VERSION" | sed 's/[()\/\._-]//g')
CALLING_DIRECTORY=$(pwd)

SYSTEM=$SYSTEM_TYPE-$SYSTEM_VERSION

function printHelp {
	echo "build.sh Options:"
	echo "   -h : Display this menu"
	echo "   -d : Build Doxygen documentation"
	echo "   -t : Build unit/performance tests"
	echo "   -c : Clean all builds"
	echo "   -r : Run unit/performance tests"
	echo "   -R params : Run unit/performance tests with the given parameters"
	echo "               (See ctest man page for parameters)"
	echo "   -b debug|release|all : Build the project in the selected configuration"
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
RUN_TESTS=
TEST_PARAMETERS=
BUILD_TYPE=
BUILD_APPS=

if [ "$#" -eq "0" ]
then
	printHelp
	exit 1
fi

while getopts "hdatcrR:b:" OPTION
do
	case $OPTION in
		h)
			printHelp
			exit 1
			;;
		d)
			BUILD_DOXYGEN="TRUE"
			;;
		a)
			BUILD_APPS="TRUE"
			;;
		t)
			BUILD_TESTS="TRUE"
			;;
		r)
			RUN_TESTS="TRUE"
			;;
		R)
			RUN_TESTS="TRUE"
			TEST_PARAMETERS=$OPTARG
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
	echo "STATUS: Cleaning build directory"
	cd $CALLING_DIRECTORY
	rm -Rf build
	echo "STATUS: Done!"
fi

if [ "$BUILD_TESTS" = "TRUE" ]
then
	export EVA_BUILD_TESTS="1"
fi

if [ "$BUILD_APPS" = "TRUE" ]
then
	export EVA_BUILD_APPS="1"
fi

if [ "$BUILD_TYPE" = "debug" -o "$BUILD_TYPE" = "all" ]
then
	buildDebug
fi

if [ "$BUILD_TYPE" = "release" -o "$BUILD_TYPE" = "all" ]
then
	buildRelease
fi

if [ "$RUN_TESTS" = "TRUE" ]
then
	echo "STATUS: Running tests"
	cd $CALLING_DIRECTORY
	if [ -d "build/$SYSTEM/release" ]
	then
		cd build/$SYSTEM/release/tests
		ctest $TEST_PARAMETERS
	elif [ -d "build/$SYSTEM/debug" ]
	then
		cd build/$SYSTEM/debug/tests
		ctest $TEST_PARAMETERS
	else
		echo "STATUS: No build detected. Testing skipped."
	fi
	echo "STATUS: Done!"
fi

if [ "$BUILD_DOXYGEN" = "TRUE" -a "$(which doxygen)" != "" ]
then
	echo "STATUS: Generating Doxygen documents"
	cd $CALLING_DIRECTORY
	if [ ! -d "build" ]
	then
		mkdir -p build
	fi
	cd src
	doxygen Doxyfile
	echo "STATUS: Done!"
fi
