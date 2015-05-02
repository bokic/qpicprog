TEMPLATE     = app
CONFIG      += debug_and_release
TARGET       = qpicprog
DESTDIR      = ../../bin

INCLUDEPATH += ../lib ../common
DEPENDPATH  += ../lib ../common

SOURCES      = main.cpp ../common/qhexloader.cpp
HEADERS      =          ../common/qhexloader.h

LIBS        += -L../../bin -lpicprog
