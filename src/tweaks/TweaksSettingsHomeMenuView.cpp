#include "TweaksSettingsHomeMenuView.h"

#include <iostream>

using namespace std;

TweaksSettingsHomeMenuView::TweaksSettingsHomeMenuView(QWidget *parent=0)
    : QWidget(parent)
{
    setupUi(this);
    cout << "TweaksSettingsHomeMenuView()" << endl << flush; 

    //connect(red,   SIGNAL(toggled(bool)), this, SLOT(setRedLed(bool)));
}

