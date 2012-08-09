#!/bin/bash -x

function do_build_and_zip ()
{
	pixfc_dir="$1"
	cmake_toolchain_file="$2"
	version="$3"
	os="$4"
	arch="$5"
	num_cpus=$6
	build_dir="${pixfc_dir}/build/temp"

	# create temp build dir
	rm -Rf "${build_dir}"
	mkdir "${build_dir}"

	# Go to pixfc build dir
	pushd "${build_dir}"
	if [ $? -ne 0 ]; then
		echo "Cant change to temp build directory ${build_dir}"
		popd
		exit 1
	fi

	# Build pixfc
	cmake "-DCMAKE_TOOLCHAIN_FILE=../../cmake/${cmake_toolchain_file}"  ../.. || { echo "CMake failed"; popd; exit 1; }
	make -j ${num_cpus} || { echo "Build failed"; popd; exit 1;  }

	# Go back to where we were invoked from
	popd

	destdir="PixFC-SSE-${version}"

	# create temp dir
	rm -Rf "${destdir}"
	mkdir "${destdir}"

	# copy library and header files
	cp "${build_dir}/src/libpixfc-sse.a" "${destdir}"
	mkdir "${destdir}/include"
	cp "${pixfc_dir}/include/pixfc-sse.h" "${destdir}/include"
	cp README.${os} "${destdir}/README"
	cp COPYING "${destdir}/"
	cp "${pixfc_dir}/example.c" "${destdir}/"
	cp "${pixfc_dir}/Changelog" "${destdir}/"
	cp "${build_dir}/tools/time_conversions" "${destdir}/"

	# create tar file
	tar -zcvf "${destdir}_${os}-${arch}.tar.gz" "${destdir}"

	rm -Rf "${destdir}"
}

# Check arguments
if [ $# -ne 3 ]; then
	echo "$0 <pixfc dir> <version tag> <num cpus>"
	exit 1
fi

# check OS
echo $OSTYPE | grep -q "linux"
if [ $? -eq 0 ]; then
		#do Linux 64-bit build
		do_build_and_zip "$1" "Toolchain-linux-x86_64-gcc.cmake" $2 linux x86_64 $3

		#do Linux 32-bit build
		do_build_and_zip "$1" "Toolchain-linux-x86-gcc.cmake" $2 linux x86 $3
else
		#do osx 64-bit build
		do_build_and_zip "$1" "Toolchain-darwin-x86_64-clang.cmake" $2 osx x86_64 $3
fi

