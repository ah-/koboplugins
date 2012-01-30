#include "TweaksSettingsView.h"

#include <iostream>

using namespace std;

TweaksSettingsView::TweaksSettingsView(QWidget *parent=0)
    : QWidget(parent)
{
    setupUi(this);

    cout << "TweaksSettingsView()" << endl << flush; 
}

