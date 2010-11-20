#!/bin/sh

rm -rf doc/devel
sed "s/@VERSION@/$(git describe)/" doc/Doxyfile.in > doc/Doxyfile
if [ "$1" = "--check" ]; then
	# ignore undocumented methods, but warn about anything undocumented in the idl
	# file or any undocumented classes
	doxygen doc/Doxyfile 2>&1|egrep -i 'warning:.*compound|idl.*warning'
else
	doxygen doc/Doxyfile
	cd doc/devel
	make
	mv refman.pdf ../devel.pdf
	echo "Developer documentation is ready in doc/devel.pdf."
fi
