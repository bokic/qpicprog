TEMPLATE           = lib
QT                 =
CONFIG            += debug_and_release
TARGET             = picprog
DESTDIR            = ../../bin

win32:DEPENDPATH  += . ../../libs/ftdi_win
win32:INCLUDEPATH += . ../../libs/ftdi_win
win64:DEPENDPATH  += . ../../libs/ftdi_win
win64:INCLUDEPATH += . ../../libs/ftdi_win
unix:INCLUDEPATH  += . ../../libs/ftdi_linux
unix:INCLUDEPATH  += . ../../libs/ftdi_linux

DEFINES           += LIB_EXPORT

# Input
SOURCES            = \
                    picprog_16f87x.c \
                    picprog.c \
                    picprog_ds33_ds24.c \
                    picprog_ds30.c
HEADERS            = \
                    picprog_16f87x.h \
                    picprog.h \
                    picprog_ds33_ds24.h \
                    picprog_ds30.h

win32:SOURCES     += advanced_delay_win.c
unix:SOURCES      += advanced_delay_unix.c
HEADERS           += advanced_delay.h
HEADERS           += export.h

win32:LIBS        += ../../libs/ftdi_win/win32/ftd2xx.lib
win64:LIBS        += ../../libs/ftdi_win/win64/ftd2xx.lib
unix:LIBS         += -L../../libs/ftdi_linux/32 -lftd2xx

win32: LIBS       += advapi32.lib
