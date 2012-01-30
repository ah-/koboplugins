#ifndef __TWEAKS_WIDGET_H__
#define __TWEAKS_WIDGET_H__

#include <QtGui>
#include "PluginInterface.h"

#include "ui_tweakview.h"

class TweaksPlugin;

class TweaksWidget : public QWidget, private Ui::TweakView
{
	Q_OBJECT

public:
	TweaksWidget(TweaksPlugin *plugin, QWidget *parent);

    bool event(QEvent* event);

public slots:
    void onResume();

    void goToNextPage();
    void goToPrevPage();

    void onDisplayMenuRequested(QList<PluginInterface::MenuGroup>&);
    void displayTitlePage();

    void homeKeyPressed();

private slots:
    void redLedOn();
    void redLedOff();
    void greenLedOn();
    void greenLedOff();
    void blueLedOn();
    void blueLedOff();

signals:
    void setHeaderText(const QString&);
    void openFooterMenu();
    void closeFooterMenu();
    void footerEvent(const QString& str, int a, int b);
    void showFooter();
    void hideFooter();
    void startedLoading();
    void doneLoading();
    void allHighlightsLoaded();
    void processTap(QPoint);
    //void processTapAndHold(TapGesture *);
    //void processSwipe(SwipeGesture *);
    void showHideDogEar();
    void hideSplash();

private:
    TweaksPlugin *plugin;
};

#endif
