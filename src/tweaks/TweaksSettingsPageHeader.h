#ifndef __TWEAKS_SETTINGS_PAGE_HEADER_H__
#define __TWEAKS_SETTINGS_PAGE_HEADER_H__

#include <QtGui>

#include "ui_TweaksSettingsPageHeader.h"

class TweaksSettingsPageHeader : public QWidget, public Ui::TweaksSettingsPageHeader
{
	Q_OBJECT

public:
	TweaksSettingsPageHeader(QWidget *parent);

signals:
    void back();
};

#endif
