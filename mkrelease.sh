#!/bin/bash -e

rm -rf sql-version-control-1.0-release
mkdir -p sql-version-control-1.0-release/{doc,src,bin}
# clean doc
(cd doc; git clean -x -d -f)
# build doc figures
make -C doc layers.pdf
make -C doc classdiagram.pdf
# build doc
make -C doc spec.pdf
make -C doc rendszerterv.pdf
sh mkdeveldoc.sh
make -C doc install.pdf
make -C doc user.pdf
# install doc
for i in spec rendszerterv devel install user
do
	cp doc/$i.pdf sql-version-control-1.0-release/doc/
done
# build source code
make
make debs
# FIXME make nsis
# install source code
make srctarball
cp sql-version-control-1.0.tar.gz sql-version-control-1.0-release/src/
# install binaries
cp sql-version-control-{gui,server}-1.0_i386.deb sql-version-control-1.0-release/bin/
# FIXME install nsis
# create release zip
zip -r sql-version-control-1.0-release.zip sql-version-control-1.0-release
rm -rf sql-version-control-1.0-release
echo "Release zip file created: sql-version-control-1.0-release.zip."
