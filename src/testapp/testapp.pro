TEMPLATE  = app
CONFIG   += debug_and_release
TARGET    = testapp
DESTDIR   = ../../bin

#RC_FILE   = testapp.rc

INCLUDEPATH += ../lib ../common
DEPENDPATH  += ../lib ../common

SOURCES   = main.cpp qmaindialog.cpp ../common/qhexloader.cpp
HEADERS   =          qmaindialog.h   ../common/qhexloader.h
FORMS    += qmaindialog.ui
RESOURCES = resource.qrc

LIBS     += -L../../bin -lpicprog
