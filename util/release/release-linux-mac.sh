#!/bin/bash -x

function do_build_and_zip ()
{
	pixfc_dir="$1"
	cmake_arg="$2"
	version="$3"
	os="$4"
	arch="$5"
	build_dir="${pixfc_dir}/build"

	# Go to pixfc build dir
	pushd "${build_dir}"
	if [ $? -ne 0 ]; then
		echo "Cant change to directory ${build_dir}"
		exit 1
	fi

	# create temp build dir
	build_dir="${build_dir}/temp"
	rm -Rf "${build_dir}"
	mkdir "${build_dir}"
	cd "${build_dir}" || { echo "Cant change to directory ${build_dir}"; popd; exit 1; }

	# Build pixfc
	cmake "${cmake_arg}"  ../.. || { echo "CMake failed"; popd; exit 1; }
	make || { echo "Build failed"; popd; exit 1;  }

	# Go back to where we were invoked from
	popd

	destdir="pixfc-sse-v${version}"

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
	cp "${build_dir}/tools/unit-testing" "${destdir}/"

	# create tar file
	tar -zcvf "${destdir}_${os}-${arch}.tar.gz" "${destdir}"

	rm -Rf "${destdir}"
}

# Check arguments
if [ $# -ne 2 ]; then
	echo "$0 <pixfc dir> <version tag>"
	exit 1
fi

# check OS
os="linux"
echo $OSTYPE | grep -q "linux" || { os="osx"; }

# check arch
native_arch="x86"
other_arch="x86_64"
other_arch_cc_compile_flag="-m64"
echo $MACHTYPE | grep -q "x86_64" && { native_arch="x86_64"; other_arch="x86"; other_arch_cc_compile_flag="-m32"; }

# do build	
do_build_and_zip $1 "" $2 "${os}" "${native_arch}"
do_build_and_zip $1 "-DCMAKE_C_FLAGS=${other_arch_cc_compile_flag}" $2 "${os}" "${other_arch}"

