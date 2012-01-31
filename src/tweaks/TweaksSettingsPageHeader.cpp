#include "TweaksSettingsPageHeader.h"

#include <iostream>

using namespace std;

TweaksSettingsPageHeader::TweaksSettingsPageHeader(QWidget *parent=0)
    : QWidget(parent)
{
    cout << "TweaksSettingsPageHeader()" << endl << flush; 
    setupUi(this);
}

