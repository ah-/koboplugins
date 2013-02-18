#include "TweaksSettingsMiscView.h"

#include <iostream>
#include "LedManager.h"

using namespace std;

TweaksSettingsMiscView::TweaksSettingsMiscView(TweaksPlugin *plugin, QWidget *parent)
    : QWidget(parent),
      plugin(plugin)
{
    setupUi(this);
    cout << "TweaksSettingsMiscView()" << endl << flush; 

    hideShop->setChecked(plugin->settings()->value("Tweaks/hideShop", false).toBool());
    hideWishlist->setChecked(plugin->settings()->value("Tweaks/hideWishlist", false).toBool());
    uninstallButton->setAttribute(Qt::WA_AcceptTouchEvents);

    connect(hideShop, SIGNAL(toggled(bool)), plugin, SLOT(hideShop(bool)));
    connect(hideWishlist, SIGNAL(toggled(bool)), plugin, SLOT(hideWishlist(bool)));
    connect(uninstallButton, SIGNAL(clicked()), plugin, SLOT(uninstallPlugin()));
}
