NICKEL_DIR = ../..

CONFIG += noqca

QT += script

TEMPLATE = lib
CONFIG += plugin \
          resources

INCLUDEPATH += ../../include

TARGET = tweaks
FORMS = tweakview.ui \
    TweaksSettingsPageView.ui \
    TweaksSettingsView.ui

SOURCES += TweaksPlugin.cpp \
    TweaksWidget.cpp \
    TweaksSettingsPageView.cpp \
    TweaksSettingsView.cpp \
    config.cpp

RESOURCES += ../../res/koboplugins.qrc

HEADERS += ../../include/PluginInterface.h \
    TweaksPlugin.h \
    TweaksWidget.h \
    TweaksSettingsPageView.h \
    TweaksSettingsView.h \
    config.h 

TRANSLATIONS = koboplugins_de.ts \
               koboplugins_fr.ts
