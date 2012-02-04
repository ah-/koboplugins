#ifndef __TWEAKS_SETTINGS_HOME_MENU_VIEW_H__
#define __TWEAKS_SETTINGS_HOME_MENU_VIEW_H__

#include <QtGui>

#include "ui_TweaksSettingsHomeMenuView.h"

class TweaksSettingsHomeMenuView : public QWidget, private Ui::TweaksSettingsHomeMenuView
{
	Q_OBJECT

public:
	TweaksSettingsHomeMenuView(QWidget *parent);
};

#endif
