QT       += core network

TARGET = Run
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    client.cpp

HEADERS += \
    server.h \
    client.h
