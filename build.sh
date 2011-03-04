#!/bin/bash

SYSTEM_TYPE=$(uname -s)
SYSTEM_VERSION=$(uname -r)
SYSTEM=$SYSTEM_TYPE-$SYSTEM_VERSION

function buildDebug {
	export CMAKE_C_FLAGS="-g -Wall"
	export CMAKE_CXX_FLAGS="-g -Wall"
	if [ ! -d "build/$SYSTEM/debug" ]
	then
		mkdir -p build/$SYSTEM/debug
	fi
	cd build/$SYSTEM/debug
	cmake ../../../src -DCMAKE_BUILD_TYPE=Debug $1
	make
}

function buildRelease {
	export CMAKE_C_FLAGS="-Wall"
	export CMAKE_CXX_FLAGS="-Wall"
	if [ ! -d "build/$SYSTEM/release" ]
	then
		mkdir -p build/$SYSTEM/release
	fi
	cd build/$SYSTEM/release
	cmake ../../../src -DCMAKE_BUILD_TYPE=Release $1
	make
}

if [ "$1" = "debug" ]
then
	buildDebug
else
	if [ "$1" = "release" ]
	then
		buildRelease
	else
		if [ "$1" = "clean" ]
		then
			echo "CLEAN BUILD"
			rm -Rf build
		else
			if [ "$1" = "test" ]
			then
				if [ -d "build/$SYSTEM/release" ]
				then
					buildRelease -BUILD_TESTS=1
					cd tests
					ctest -V
				elif [ -d "build/$SYSTEM/debug" ]
				then
					buildDebug
					cd tests
					ctest -V
				else
					echo "No build detected. Canceling test."
				fi
			else
				if [ "$1" = "all" ]
				then
					./build.sh debug
					./build.sh release
				else
					if [ "$1" = "doxygen" ]
					then
						cd src
						doxygen Doxyfile
					else
						echo "Invalid option '$1' chosen. Terminating script." 
					fi
				fi
			fi
		fi	
	fi
fi

