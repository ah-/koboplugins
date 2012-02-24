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

    recommendations->setChecked(plugin->settings()->value("Tweaks/hideRecommendations", false).toBool());
    shelves->setChecked(plugin->settings()->value("Menu/showShelves", false).toBool());
    uninstallButton->setAttribute(Qt::WA_AcceptTouchEvents);

    connect(recommendations, SIGNAL(toggled(bool)), plugin, SLOT(hideRecommendations(bool)));
    connect(shelves, SIGNAL(toggled(bool)), plugin, SLOT(enableShelves(bool)));
	connect(uninstallButton, SIGNAL(clicked()), plugin, SLOT(uninstallPlugin()));
}
