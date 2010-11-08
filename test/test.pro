CONFIG += debug
CONFIG += qt
QT += sql
QT -= gui
SOURCES += test.cc ../stub/VersionControlSK.cc ../server/server_lib.cc ../server/AuthImpl.cc ../server/RootImpl.cc ../server/UserAdminImpl.cc ../server/ModelReader.cc  ../server/RevisionImpl.cc ../server/UserImpl.cc ../server/ModelWriter.cc ../server/AdminImpl.cc ../server/ModelAdminImpl.cc ../server/ResolverImpl.cc
HEADERS += ../stub/VersionControl.hh ../server/server_lib.hh test.hh AuthTest.hh AdminTest.hh RootTest.hh ModelAdminTest.hh ResolverTest.hh ModelTest.hh RevisionTest.hh UserTest.hh
INCLUDEPATH += ../stub ../server ../xdiff
LIBS += -lomniORB4 -lcppunit -ldl ../xdiff/libxdiff.a
