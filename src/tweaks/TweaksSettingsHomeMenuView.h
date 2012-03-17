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
    void on_shortlist_toggled(bool);
    void on_dictionary_toggled(bool);
    void on_readingLife_toggled(bool);
    void on_search_toggled(bool);
    void on_store_toggled(bool);
    void on_sync_toggled(bool);
    void on_help_toggled(bool);
    void on_browser_toggled(bool);
    void on_airplaneMode_toggled(bool);
    void on_toggleWifi_toggled(bool);
    void on_powerOff_toggled(bool);
    void on_sleep_toggled(bool);

private:
    TweaksPlugin *plugin;
};

#endif
