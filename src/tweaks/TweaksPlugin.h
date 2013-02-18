#ifndef __TWEAKS_PLUGIN_H__
#define __TWEAKS_PLUGIN_H__

#include <QImageIOPlugin>
#include <QtGui>
#include <QtScript>

#include "../../include/PluginInterface.h"
#include "TweaksSettingsPageView.h"

#include "MenuEntry.h"

class N3SettingsExtrasController;
class TweaksSettingsView;

#define CONFIG_PATH "/mnt/onboard/.kobo/kobotweaks.ini"

class TweaksPlugin : public QImageIOPlugin
{
    Q_OBJECT

public:
    TweaksPlugin(QObject *parent = 0);
    ~TweaksPlugin();

    // Fake QImageIOPlugin implementation
    Capabilities capabilities(QIODevice *device, const QByteArray &format) const;
    QImageIOHandler* create(QIODevice *device, const QByteArray &format = QByteArray()) const;
    QStringList keys() const;

    void initialize();

    QSharedPointer<QSettings> settings();

public slots:
    void focusChanged(QWidget *old, QWidget *now);
    void windowChanged(int index);
    void open(QString mimeType);

    void enableBrowserShortcut(bool enable);

    void hideShop(bool enable);
    void hideWishlist(bool enable);

    void patchReadingLife();

    void library();
    void tweaksSettings();
    void openBrowser();
    void connectWifi();
    void toggleAirplaneMode();
    void uninstallPlugin();
    void sync(bool);
    void sleep();
    void powerOff();
    void shelves();
    void readingLife();
    void sudoku();
    void chess();
    void scribble();
    void executeSystemCmd(QString cmd);

private:
    bool checkFirmwareVersion();

private:
    QSharedPointer<QSettings> pluginSettings;
    QScriptEngine engine;
    QSignalMapper mapper;
    QStackedWidget *stackedWidget;
    N3SettingsExtrasController *n3SettingsExtrasController;
    QList<MenuEntry> menuEntries;
};

#endif
