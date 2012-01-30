#ifndef __TWEAKS_SETTINGS_VIEW_H__
#define __TWEAKS_SETTINGS_VIEW_H__

#include <QtGui>

#include "ui_TweaksSettingsView.h"

class TweaksSettingsView : public QWidget, private Ui::TweaksSettingsView
{
	Q_OBJECT

public:
	TweaksSettingsView(QWidget *parent);
};

#endif
