#include "TweaksSettingsView.h"

#include <iostream>
#include "MainWindowController.h"
#include "TweaksSettingsLEDView.h"
#include "TweaksSettingsPageView.h"

using namespace std;

TweaksSettingsView::TweaksSettingsView(QWidget *parent=0)
    : QWidget(parent)
{
    setupUi(this);
    cout << "TweaksSettingsView()" << endl << flush; 
    connect(leds, SIGNAL(tapped()), this, SLOT(led()));
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
}
