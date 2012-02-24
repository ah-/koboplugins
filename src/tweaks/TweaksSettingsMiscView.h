#ifndef __TWEAKS_SETTINGS_MISC_VIEW_H__
#define __TWEAKS_SETTINGS_MISC_VIEW_H__

#include <QtGui>

#include "TweaksPlugin.h"

#include "ui_TweaksSettingsMiscView.h"

class TweaksSettingsMiscView : public QWidget, private Ui::TweaksSettingsMiscView
{
	Q_OBJECT

public:
	TweaksSettingsMiscView(TweaksPlugin *plugin, QWidget *parent);

private:
    TweaksPlugin *plugin;
};

#endif
