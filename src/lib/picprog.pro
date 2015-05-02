TEMPLATE        = lib
QT              =
CONFIG         += debug_and_release

DEPENDPATH     += .
INCLUDEPATH    += .

DEFINES        += LIB_EXPORT

# Input
SOURCES         = picprog_78x.c picprog_ds.c
HEADERS         = picprog_78x.h picprog_ds.h

win32:SOURCES  += advanced_delay_win.c
unix:SOURCES   += advanced_delay_unix.c
HEADERS        += advanced_delay.h
HEADERS        += export.h
