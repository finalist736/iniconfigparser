TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    iniconfig.c

HEADERS += \
    iniconfig.h

DISTFILES += \
    test.ini
