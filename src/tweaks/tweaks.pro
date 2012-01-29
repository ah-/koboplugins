NICKEL_DIR = ../..

CONFIG += noqca

QT += script

TEMPLATE = lib
CONFIG += plugin \
          resources

TARGET = tweaks
FORMS = tweakview.ui

SOURCES += TweaksPlugin.cpp \
    TweaksWidget.cpp \
    config.cpp

RESOURCES += ../../res/koboplugins.qrc

HEADERS += ../../include/PluginInterface.h \
    TweaksPlugin.h \
    TweaksWidget.h \
    config.h 

TRANSLATIONS = koboplugins_de.ts \
               koboplugins_fr.ts
