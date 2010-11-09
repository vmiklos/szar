TEMPLATE = app
FORMS += *.ui
SOURCES += *.cpp ../stub/VersionControlSK.cc
HEADERS += ../stub/VersionControl.hh
INCLUDEPATH += ../stub
LIBS += -lomniORB4
CONFIG += qt
