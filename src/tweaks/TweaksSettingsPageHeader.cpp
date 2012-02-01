#include "TweaksSettingsPageHeader.h"

#include <iostream>

using namespace std;

TweaksSettingsPageHeader::TweaksSettingsPageHeader(QWidget *parent=0)
    : QWidget(parent)
{
    cout << "TweaksSettingsPageHeader()" << endl << flush; 
    setupUi(this);

    backLabel->setSelectedPixmap(":/images/search/trilogy_icon_circle_arrow_hit.png");
    backLabel->setDeselectedPixmap(":/images/search/trilogy_icon_circle_arrow.png");
    backLabel->repaint();

    connect(backLabel, SIGNAL(tapped()), this, SIGNAL(back()));
}
