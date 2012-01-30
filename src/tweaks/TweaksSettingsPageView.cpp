#include "TweaksSettingsPageView.h"

#include <iostream>
#include "MainWindowController.h"
#include "TweaksSettingsView.h"

using namespace std;

TweaksSettingsPageView::TweaksSettingsPageView(QWidget *parent=0)
    : QWidget(parent)
{
    cout << "TweaksSettingsPageView()" << endl << flush; 
    setupUi(this);

    TweaksSettingsView *settingsView = new TweaksSettingsView(this);
    setContentWidget(settingsView);
}

void TweaksSettingsPageView::changeEvent(QEvent *e)
{
    cout << "TweaksSettingsPageView::changeEvent(QEvent *e)" << endl << flush;
}

void TweaksSettingsPageView::dismissDialog()
{
    cout << "TweaksSettingsPageView::dismissDialog()" << endl << flush;
    MainWindowController::sharedInstance()->popView(this);
}

QWidget *TweaksSettingsPageView::getContentWidget()
{
    return contentWidget;
}

void TweaksSettingsPageView::setContentWidget(QWidget *w)
{
    contentWidget = w;
    contentLayout->addWidget(w);
}
