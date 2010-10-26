CONFIG += qt
QT += sql
QT -= gui
SOURCES += *.cc ../stub/VersionControlSK.cc
HEADERS += ../stub/VersionControl.hh server_lib.hh
INCLUDEPATH += ../stub
LIBS += -lomniORB4
