TEMPLATE  = app
CONFIG   += debug_and_release
TARGET    = qpicprog
DESTDIR   = ../../bin

RC_FILE   = qpicprog.rc

INCLUDEPATH += ../lib
DEPENDPATH  += ../lib

SOURCES   = main.cpp qmaindialog.cpp qhexloader.cpp
HEADERS   =          qmaindialog.h   qhexloader.h
FORMS    += qmaindialog.ui
RESOURCES = resource.qrc

LIBS     += -L../../bin -lpicprog
