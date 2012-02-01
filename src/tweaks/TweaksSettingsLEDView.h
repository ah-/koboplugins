#ifndef __TWEAKS_SETTINGS_LED_VIEW_H__
#define __TWEAKS_SETTINGS_LED_VIEW_H__

#include <QtGui>

#include "ui_TweaksSettingsLEDView.h"

class TweaksSettingsLEDView : public QWidget, private Ui::TweaksSettingsLEDView
{
	Q_OBJECT

public:
	TweaksSettingsLEDView(QWidget *parent);

public slots:
    void setRedLed(bool enable);
    void setGreenLed(bool enable);
    void setBlueLed(bool enable);
};

#endif
