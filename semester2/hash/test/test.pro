include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    tester.h

SOURCES += \
        main.cpp \
    ../array.c \
    ../double_hash.c
