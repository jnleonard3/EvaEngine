SYSTEM_TYPE=$(uname -s)
SYSTEM_TYPE=$(echo "$SYSTEM_TYPE" | sed 's/[()\/\._-]//g')
SYSTEM_VERSION=$(uname -r)
SYSTEM_VERSION=$(echo "$SYSTEM_VERSION" | sed 's/[()\/\._-]//g')

SYSTEM=$SYSTEM_TYPE-$SYSTEM_VERSION

function buildDebug {
	export CMAKE_C_FLAGS="-g -Wall"
	export CMAKE_CXX_FLAGS="-g -Wall"
	if [ ! -d "build/$SYSTEM/debug" ]
	then
		mkdir -p build/$SYSTEM/debug
	fi
	cd build/$SYSTEM/debug
	cmake ../../../src -DCMAKE_BUILD_TYPE=Debug
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
	cmake ../../../src -DCMAKE_BUILD_TYPE=Release
	make
}

if [ -d "build/$SYSTEM/release" ]
then
	buildRelease
	cd tests
	ctest $1 $2 $3
elif [ -d "build/$SYSTEM/debug" ]
then
	buildDebug
	cd tests
	ctest $1 $2 $3
else
	echo "No build detected. Canceling testing."
fi


