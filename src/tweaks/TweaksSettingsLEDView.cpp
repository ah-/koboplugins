#include "TweaksSettingsLEDView.h"

#include <iostream>
#include "LedManager.h"

using namespace std;

TweaksSettingsLEDView::TweaksSettingsLEDView(QWidget *parent=0)
    : QWidget(parent)
{
    setupUi(this);
    cout << "TweaksSettingsLEDView()" << endl << flush; 

    connect(red,   SIGNAL(toggled(bool)), this, SLOT(setRedLed(bool)));
    connect(green, SIGNAL(toggled(bool)), this, SLOT(setGreenLed(bool)));
    connect(blue,  SIGNAL(toggled(bool)), this, SLOT(setBlueLed(bool)));

    LedManager::sharedInstance()->off();
}

void TweaksSettingsLEDView::setRedLed(bool enable)
{
    if (enable)
        LedManager::sharedInstance()->redOn();
    else
        LedManager::sharedInstance()->redOff();
}

void TweaksSettingsLEDView::setGreenLed(bool enable)
{
    if (enable)
        LedManager::sharedInstance()->greenOn();
    else
        LedManager::sharedInstance()->greenOff();
}

void TweaksSettingsLEDView::setBlueLed(bool enable)
{
    if (enable)
        LedManager::sharedInstance()->blueOn();
    else
        LedManager::sharedInstance()->blueOff();
}
