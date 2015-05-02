# -------------------------------------------------
# Project created by QtCreator 2009-04-28T19:46:32
# -------------------------------------------------
# QT += network
TEMPLATE = app
CONFIG += debug_and_release
TARGET = qtpicprog

RC_FILE = QtPicProg.rc

SOURCES += \
	main.cpp \
    qmaindialog.cpp \
    qpicprog.cpp \
    qds33picprog.cpp \
    q16f87xpicprog.cpp \
    qhexloader.cpp

win32:SOURCES += qadvancedtimer_win.cpp
unix:SOURCES  += qadvancedtimer_unix.cpp

HEADERS += \
	qmaindialog.h \
    qadvancedtimer.h \
    qpicprog.h \
    qds33picprog.h \
    q16f87xpicprog.h \
    qhexloader.h
FORMS += qmaindialog.ui
RESOURCES += resource.qrc
