#ifndef __TWEAKS_SETTINGS_VIEW_H__
#define __TWEAKS_SETTINGS_VIEW_H__

#include <QtGui>

#include "TweaksPlugin.h"

#include "ui_TweaksSettingsView.h"

class TweaksSettingsView : public QWidget, private Ui::TweaksSettingsView
{
	Q_OBJECT

public:
	TweaksSettingsView(TweaksPlugin *plugin, QWidget *parent);

public slots:
    void led();
    void misc();
    void homeMenu();

private:
    TweaksPlugin *plugin;
};

#endif
