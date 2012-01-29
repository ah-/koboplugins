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
    config.h \
    ../../include/Content.h \
    ../../include/SignalRegistry.h \
    ../../include/ReadingViewMixin.h \
    ../../include/MenuFactory.h \
    ../../include/WirelessWorkflowManager.h \
    ../../include/HomeMenuController.h \
    ../../include/MassStorageManager.h \
    ../../include/Volume.h \
    ../../include/SpinnerManager.h \
    ../../include/WirelessWatchdog.h \
    ../../include/QtScriptPluginInterface.h \
    ../../include/NavigationController.h \
    ../../include/WirelessManager.h \
    ../../include/LedManager.h \
    ../../include/N3FSSyncManager.h \
    ../../include/MainWindowController.h \
    ../../include/N3ReaderOpener.h \
    ../../include/HomePageGridView.h \
    ../../include/PluginLoader.h \
    ../../include/N3SettingsController.h

TRANSLATIONS = koboplugins_de.ts \
               koboplugins_fr.ts