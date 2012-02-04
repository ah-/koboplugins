#include "TweaksSettingsHomeMenuView.h"

#include <iostream>

using namespace std;

TweaksSettingsHomeMenuView::TweaksSettingsHomeMenuView(TweaksPlugin *plugin, QWidget *parent=0)
    : QWidget(parent),
      plugin(plugin)
{
    setupUi(this);
    cout << "TweaksSettingsHomeMenuView()" << endl << flush; 

    shortlist->setChecked(plugin->settings()->value("Menu/showShortlist", false).toBool());
    customMenu->setChecked(plugin->settings()->value("Menu/customMenuEnabled", false).toBool());
}

void TweaksSettingsHomeMenuView::customMenuEnabled(bool enabled) {
    plugin->settings()->setValue("Menu/customMenuEnabled", customMenu->isChecked());
}

void TweaksSettingsHomeMenuView::on_shortlist_stateChanged(int state) {
    plugin->settings()->setValue("Menu/showShortlist", shortlist->isChecked());
    cout << "shortlist" << endl << flush;
}

void TweaksSettingsHomeMenuView::showDictionary(bool enabled) {
    plugin->settings()->setValue("Menu/showDictionary", dictionary->isChecked());
}

void TweaksSettingsHomeMenuView::showReadingLife(bool enabled) {
    plugin->settings()->setValue("Menu/showReadingLife", readingLife->isChecked());
}

void TweaksSettingsHomeMenuView::showStore(bool enabled) {
    plugin->settings()->setValue("Menu/showStore", store->isChecked());
}

void TweaksSettingsHomeMenuView::showSync(bool enabled) {
    plugin->settings()->setValue("Menu/showSync", sync->isChecked());
}

void TweaksSettingsHomeMenuView::showHelp(bool enabled) {
    plugin->settings()->setValue("Menu/showHelp", help->isChecked());
}

void TweaksSettingsHomeMenuView::showBrowser(bool enabled) {
}

void TweaksSettingsHomeMenuView::showAirplaneMode(bool enabled) {
}

void TweaksSettingsHomeMenuView::showWifiOnOff(bool enabled) {
}

void TweaksSettingsHomeMenuView::showPowerOff(bool enabled) {
}
