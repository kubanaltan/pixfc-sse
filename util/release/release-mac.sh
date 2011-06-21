#!/bin/bash

if [ $# -ne 2 ]; then
	echo "$0 <pixfc dir> <version tag>"
	exit 1
fi

# Go to pixfc build dir
pushd $1/build
if [ $? -ne 0 ]; then
	echo "Cant change to directory $1/build"
	exit 1
fi

# Build pixfc
cmake .. || { echo "CMake failed"; popd; exit 1; }
make || { echo "Build falied"; popd; exit 1;  }

# Go back to where we were invoked from
popd

destdir=pixfc-sse_v$2

# create temp dir
rm -Rf ${destdir}
mkdir ${destdir}

# copy library and header files
cp $1/build/src/libpixfc-sse.a ${destdir}
mkdir ${destdir}/include
cp $1/include/pixfc-sse.h ${destdir}/include
cp README.osx ${destdir}/README
cp COPYING ${destdir}/
cp $1/example.c ${destdir}/

# create tar file
tar -zcvf pixfc-sse_v$2_MacOSX.tar.gz ${destdir}

