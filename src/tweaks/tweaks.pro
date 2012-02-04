NICKEL_DIR = ../..

CONFIG += noqca

QT += script

TEMPLATE = lib
CONFIG += plugin \
          resources

INCLUDEPATH += ../../include \
                ../../libs/common/include \
                ../../libs/support/include

;LIBS += -lsupport
;LIBS += -L../../libs/release

TARGET = tweaks
FORMS = tweakview.ui \
    TweaksSettingsPageHeader.ui \
    TweaksSettingsPageView.ui \
    TweaksSettingsView.ui \
    TweaksSettingsLEDView.ui \
    TweaksSettingsHomeMenuView.ui

SOURCES += TweaksPlugin.cpp \
    TweaksWidget.cpp \
    TweaksSettingsPageHeader.cpp \
    TweaksSettingsPageView.cpp \
    TweaksSettingsView.cpp \
    TweaksSettingsLEDView.cpp \
    TweaksSettingsHomeMenuView.cpp \
    config.cpp

RESOURCES += ../../res/koboplugins.qrc

HEADERS += ../../include/PluginInterface.h \
    TweaksPlugin.h \
    TweaksWidget.h \
    TweaksSettingsPageHeader.h \
    TweaksSettingsPageView.h \
    TweaksSettingsView.h \
    TweaksSettingsLEDView.h \
    TweaksSettingsHomeMenuView.h \
    config.h 

TRANSLATIONS = koboplugins_de.ts \
               koboplugins_fr.ts \
               koboplugins_es.ts
