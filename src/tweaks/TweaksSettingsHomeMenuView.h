#ifndef __TWEAKS_SETTINGS_HOME_MENU_VIEW_H__
#define __TWEAKS_SETTINGS_HOME_MENU_VIEW_H__

#include <QtGui>

#include "TweaksPlugin.h"

#include "ui_TweaksSettingsHomeMenuView.h"

class TweaksSettingsHomeMenuView : public QWidget, private Ui::TweaksSettingsHomeMenuView
{
	Q_OBJECT

public:
	TweaksSettingsHomeMenuView(TweaksPlugin *plugin, QWidget *parent);

public slots:
    void customMenuEnabled(bool);
    void showLibrary(bool);
    void on_shortlist_stateChanged(int);
    void showDictionary(bool);
    void showReadingLife(bool);
    void showStore(bool);
    void showSync(bool);
    void showHelp(bool);
    void showBrowser(bool);
    void showAirplaneMode(bool);
    void showWifiOnOff(bool);
    void showPowerOff(bool);

private:
    TweaksPlugin *plugin;
};

#endif
