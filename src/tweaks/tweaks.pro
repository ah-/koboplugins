NICKEL_DIR = ../..

CONFIG += noqca

QT += script

TEMPLATE = lib
CONFIG += plugin
TARGET = tweaks

SOURCES += TweaksPlugin.cpp \
    TweaksWidget.cpp

HEADERS += ../../include/PluginInterface.h \
    TweaksPlugin.h \
    TweaksWidget.h
