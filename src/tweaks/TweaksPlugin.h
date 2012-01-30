#ifndef __TWEAKS_PLUGIN_H__
#define __TWEAKS_PLUGIN_H__

#include <QtGui>
#include "../../include/PluginInterface.h"
#include "TweaksSettingsPageView.h"

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

    bool wirelessTimeoutEnabled();

public slots:
    void windowChanged(int index);
    void patchMenu();
    void open(QString mimeType);
    void enableBrowserShortcut(bool enable);
    void enableWirelessTimeout(bool enable);
    void hideRecommendations(bool enable);
    void sync(bool);
	void uninstallPlugin();

private:
    bool checkFirmwareVersion();

private:
    //TweaksWidget *w;
    TweaksSettingsPageView *settingsPageView;
    QSignalMapper mapper;
    QStackedWidget *sw;
    void *lastPatchedMenu;
};

#endif
