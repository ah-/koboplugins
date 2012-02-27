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
    TweaksSettingsMiscView.ui \
    TweaksSettingsHomeMenuView.ui

SOURCES += TweaksPlugin.cpp \
    TweaksSettingsPageHeader.cpp \
    TweaksSettingsPageView.cpp \
    TweaksSettingsView.cpp \
    TweaksSettingsLEDView.cpp \
    TweaksSettingsMiscView.cpp \
    TweaksSettingsHomeMenuView.cpp

RESOURCES += ../../res/koboplugins.qrc

HEADERS += ../../include/PluginInterface.h \
    TweaksPlugin.h \
    TweaksSettingsPageHeader.h \
    TweaksSettingsPageView.h \
    TweaksSettingsView.h \
    TweaksSettingsLEDView.h \
    TweaksSettingsMiscView.h \
    TweaksSettingsHomeMenuView.h

TRANSLATIONS = koboplugins.de.ts \
               koboplugins.fr.ts \
               koboplugins.it.ts \
               koboplugins.es.ts
