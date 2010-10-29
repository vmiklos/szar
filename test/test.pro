CONFIG += debug
CONFIG += qt
QT += sql
QT -= gui
SOURCES += test.cc ../stub/VersionControlSK.cc ../server/server_lib.cc ../server/AuthImpl.cc ../server/UserRoot.cc ../server/MyUserAdmin.cc ../server/ModelReader.cc  ../server/RevisionImpl.cc ../server/UserImpl.cc ../server/ModelWriter.cc
HEADERS += ../stub/VersionControl.hh ../server/server_lib.hh test.hh LoginTest.hh
INCLUDEPATH += ../stub ../server
LIBS += -lomniORB4 -lcppunit -ldl
