#ifndef __TWEAKS_PLUGIN_H__
#define __TWEAKS_PLUGIN_H__

#include <QtGui>
#include "PluginInterface.h"
#include "TweaksSettingsPageView.h"
//#include "loggerlib.h"

#define CONFIG_PATH "/mnt/onboard/.kobo/koboplugins.ini"

class TweaksPlugin : public QObject, public PluginInterface
{
Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    TweaksPlugin();
    ~TweaksPlugin();

    QStringList mimeTypes();

    QWidget *reader(void* plugin_state, QWidget *parent = 0);
    ParserInterface *parser();

    QSharedPointer<QSettings> settings();

    bool wirelessTimeoutEnabled();

public slots:
    void windowChanged(int index);
    void patchMenu();
    void patchLibraryMenu();
    void open(QString mimeType);
    void enableBrowserShortcut(bool enable);
    void enableWirelessTimeout(bool enable);
    void enableShelves(bool enable);
    void hideRecommendations(bool enable);
    void sync(bool);
    void uninstallPlugin();
    void bookFooterOpened();
    void openBrowser();

private:
    bool checkFirmwareVersion();
    bool createHomeMenuEntry(QString mapping, QString icon, QString text);
    bool createLibraryMenuEntry(QString mapping, QString text);

private:
    QSharedPointer<QSettings> pluginSettings;
    TweaksSettingsPageView *settingsPageView;
    QSignalMapper mapper;
    QStackedWidget *sw;
    void *lastPatchedMenu;
    void *lastPatchedLibraryMenu;
    //LoggerBase* m_pLogBase;
};

#endif
