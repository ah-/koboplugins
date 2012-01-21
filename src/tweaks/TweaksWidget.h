#ifndef __TWEAKS_WIDGET_H__
#define __TWEAKS_WIDGET_H__

#include <QtGui>
#include "../../include/PluginInterface.h"

class TweaksWidget : public QPushButton
{
	Q_OBJECT

public:
    TweaksWidget();
	TweaksWidget(const QString& text, QWidget *parent);

    bool event(QEvent* event);

public slots:
    void onPush(bool);

    void onResume();

    void goToNextPage();
    void goToPrevPage();

    void onDisplayMenuRequested(QList<PluginInterface::MenuGroup>&);
    void displayTitlePage();

    void homeKeyPressed();

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

};

#endif
