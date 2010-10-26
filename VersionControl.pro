TEMPLATE = subdirs
SUBDIRS = server client test
QMAKE_EXTRA_TARGETS += stub
stub.commands = mkdir -p stub && omniidl -Cstub -bcxx VersionControl.idl
