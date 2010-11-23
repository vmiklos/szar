TEMPLATE = subdirs
SUBDIRS = server client test xdiff gui
QMAKE_EXTRA_TARGETS += stub debs
stub.commands = mkdir -p stub && omniidl -Cstub -bcxx -Wbtp VersionControl.idl
debs.commands = ./builddeb.sh gui && ./builddeb.sh server
