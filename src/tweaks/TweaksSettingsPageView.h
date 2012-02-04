#ifndef __TWEAKS_SETTINGS_PAGE_VIEW_H__
#define __TWEAKS_SETTINGS_PAGE_VIEW_H__

#include <QtGui>

#include "ui_TweaksSettingsPageView.h"

class TweaksSettingsPageView : public QWidget, private Ui::TweaksSettingsPageView
{
	Q_OBJECT

public:
	TweaksSettingsPageView(QWidget *parent, QWidget *contentWidget=0);

    QWidget *getContentWidget();
    void setContentWidget(QWidget *);
    //void setHeader(QString s, bool b);

public slots:
    void dismissDialog();

private slots:

signals:
    //void setHeaderText(const QString&);

private:
    QWidget *contentWidget;
};

#endif
