#!/bin/bash

# Check arguments
if [ $# -ne 2 ]; then
	echo "$0 <pixfc dir> <version tag>"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "$1 is not a directory"
	exit 1
fi

release_name=PixFC-SSE-$2
archive_name=${release_name}_src.zip

# create temp dir
rm -Rf temp
mkdir temp
cd temp

# export working copy
svn export "../$1" "${release_name}/"
if [ $? -ne 0 ]; then
	echo "Error exporting working copy"
	exit 1
fi

# zip archive
zip -r9 ../${archive_name} ${release_name}
cd ..

rm -Rf temp

