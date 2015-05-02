TEMPLATE     = app
CONFIG      += debug_and_release
TARGET       = tests
DESTDIR      = ../../bin

INCLUDEPATH += ../lib
DEPENDPATH  += ../lib

SOURCES      = main.c

LIBS        += -L../../bin -lpicprog
