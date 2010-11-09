TEMPLATE = subdirs
SUBDIRS = server client test xdiff gui
QMAKE_EXTRA_TARGETS += stub
stub.commands = mkdir -p stub && omniidl -Cstub -bcxx -Wbtp VersionControl.idl
