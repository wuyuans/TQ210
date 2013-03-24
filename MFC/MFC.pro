QT -= core gui

TARGET = mfc

CONFIG += console

TEMPLATE = app

HEADERS += \
    mfc.h \
    SsbSipMfcApi.h \
    mfc_interface.h

SOURCES += \
    mfc.cpp \
    SsbSipMfcEncAPI.c \
    main.cpp
