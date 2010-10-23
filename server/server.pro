CONFIG += qt
QT += sql
QT -= gui
SOURCES += server.cc ../stub/VersionControlSK.cc
HEADERS += ../stub/VersionControl.hh
INCLUDEPATH += ../stub
LIBS += -lomniORB4
