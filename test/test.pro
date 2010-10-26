CONFIG += debug
SOURCES += test.cc ../stub/VersionControlSK.cc
HEADERS += ../stub/VersionControl.hh LoginTest.hh
INCLUDEPATH += ../stub
LIBS += -lomniORB4 -lcppunit -ldl
