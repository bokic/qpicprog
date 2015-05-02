TEMPLATE        = lib
QT              =
CONFIG         += debug_and_release
TARGET          = picprog
DESTDIR         = ../../bin

DEPENDPATH     += . ../../libs/ftdi
INCLUDEPATH    += . ../../libs/ftdi

DEFINES        += LIB_EXPORT

# Input
SOURCES         = \
    picprog_16f87x.c \
    picprog.c \
    picprog_ds.c
HEADERS         = \
    picprog_16f87x.h \
    picprog.h \
    picprog_ds.h

win32:SOURCES  += advanced_delay_win.c
unix:SOURCES   += advanced_delay_unix.c
HEADERS        += advanced_delay.h
HEADERS        += export.h

win32:LIBS     += ../../libs/ftdi/win32/ftd2xx.lib
win64:LIBS     += ../../libs/ftdi/win64/ftd2xx.lib
unix:LIBS      += -lftd2xx
