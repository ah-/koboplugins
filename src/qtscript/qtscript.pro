NICKEL_DIR = ../..

CONFIG += noqca

QT += script

TEMPLATE = lib
CONFIG += plugin
TARGET = qtscript

SOURCES += QtScriptPlugin.cpp \
    SocketHandler.cpp

HEADERS += ../../include/PluginInterface.h \
    QtScriptPlugin.h \
    SocketHandler.h

