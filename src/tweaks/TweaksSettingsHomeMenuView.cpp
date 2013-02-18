#include "TweaksSettingsHomeMenuView.h"

#include <iostream>

using namespace std;

TweaksSettingsHomeMenuView::TweaksSettingsHomeMenuView(TweaksPlugin *plugin, QWidget *parent=0)
    : QWidget(parent),
      plugin(plugin)
{
    setupUi(this);
    cout << "TweaksSettingsHomeMenuView()" << endl << flush; 

    //shortlist->setChecked(plugin->settings()->value("Menu/showShortlist", false).toBool());
}

//void TweaksSettingsHomeMenuView::on_shortlist_toggled(bool enabled) {
    //plugin->settings()->setValue("Menu/showShortlist", enabled);
//}
