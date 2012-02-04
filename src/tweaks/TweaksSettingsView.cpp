#include "TweaksSettingsView.h"

#include <iostream>
#include "MainWindowController.h"
#include "TweaksSettingsLEDView.h"
#include "TweaksSettingsHomeMenuView.h"
#include "TweaksSettingsPageView.h"

using namespace std;

TweaksSettingsView::TweaksSettingsView(QWidget *parent=0)
    : QWidget(parent)
{
    setupUi(this);
    cout << "TweaksSettingsView()" << endl << flush; 
    connect(ledLabel, SIGNAL(tapped()), this, SLOT(led()));
    connect(homeMenuLabel, SIGNAL(tapped()), this, SLOT(homeMenu()));
    connect(miscLabel, SIGNAL(tapped()), this, SLOT(misc()));
}

void TweaksSettingsView::led()
{
    TweaksSettingsPageView *pv = new TweaksSettingsPageView(this, new TweaksSettingsLEDView(0));
    MainWindowController *mwc = MainWindowController::sharedInstance();
    mwc->pushView(pv);
}

void TweaksSettingsView::misc()
{
}

void TweaksSettingsView::homeMenu()
{
    TweaksSettingsPageView *pv = new TweaksSettingsPageView(this, new TweaksSettingsHomeMenuView(0));
    MainWindowController *mwc = MainWindowController::sharedInstance();
    mwc->pushView(pv);
}
