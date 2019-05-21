include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
    ../data.h \
    ../stack.h \
    ../def.h \
    ../array.h \
    ../b_tree.h \
    tester.h

SOURCES += \
        main.cpp \
    ../stack.c \
    ../b_tree.c \
    ../array.c
