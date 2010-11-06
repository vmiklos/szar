CONFIG += qt
QT += sql
QT -= gui
SOURCES += *.cc ../stub/VersionControlSK.cc
HEADERS += ../stub/VersionControl.hh *.hh
INCLUDEPATH += ../stub ../xdiff
LIBS += -lomniORB4 ../xdiff/libxdiff.a
