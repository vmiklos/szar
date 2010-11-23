#!/bin/bash
EXEC=$1
APP=sql-version-control-$EXEC
if [ -e tmp ]; then echo "Please remove 'tmp' directory"; exit 1; fi
make $EXEC || exit 1
mkdir -p tmp/usr/bin
ln $EXEC/$EXEC tmp/usr/bin/$APP
if [ "$EXEC" == "server" ]; then
	mkdir -p tmp/usr/share/sql-version-control;
	ln VersionControl.sql tmp/usr/share/sql-version-control;
fi
mkdir -p tmp/DEBIAN
sed s/__IS__/`du -s tmp | cut -f 1`/ $EXEC/control >tmp/DEBIAN/control
cd tmp
md5sum usr/bin/$APP >DEBIAN/md5sums
cd ..
dpkg-deb -b tmp $APP-1.0_i386.deb
rm -rf tmp
