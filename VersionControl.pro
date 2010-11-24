TEMPLATE = subdirs
SUBDIRS = server client test xdiff gui
QMAKE_EXTRA_TARGETS += stub debs nsis srctarball
stub.commands = mkdir -p stub && omniidl -Cstub -bcxx -Wbtp VersionControl.idl
debs.commands = ./builddeb.sh gui && ./builddeb.sh server
nsis.commands = makensis VersionControl.nsi
srctarball.commands = git archive --prefix=sql-version-control-1.0/ HEAD |gzip > sql-version-control-1.0.tar.gz
