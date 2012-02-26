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
    dictionary->setChecked(plugin->settings()->value("Menu/showDictionary", false).toBool());
    readingLife->setChecked(plugin->settings()->value("Menu/showReadingLife", false).toBool());
    search->setChecked(plugin->settings()->value("Menu/showSearch", false).toBool());
    store->setChecked(plugin->settings()->value("Menu/showStore", false).toBool());
    sync->setChecked(plugin->settings()->value("Menu/showSync", false).toBool());
    help->setChecked(plugin->settings()->value("Menu/showHelp", false).toBool());
    browser->setChecked(plugin->settings()->value("Menu/showBrowser", false).toBool());
    airplaneMode->setChecked(plugin->settings()->value("Menu/showAirplaneMode", false).toBool());
    toggleWifi->setChecked(plugin->settings()->value("Menu/showWifiOnOff", false).toBool());
    powerOff->setChecked(plugin->settings()->value("Menu/showPowerOff", false).toBool());
}

void TweaksSettingsHomeMenuView::on_shortlist_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showShortlist", enabled);
}

void TweaksSettingsHomeMenuView::on_dictionary_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showDictionary", enabled);
}

void TweaksSettingsHomeMenuView::on_readingLife_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showReadingLife", enabled);
}

void TweaksSettingsHomeMenuView::on_search_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showSearch", enabled);
}

void TweaksSettingsHomeMenuView::on_store_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showStore", enabled);
}

void TweaksSettingsHomeMenuView::on_sync_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showSync", enabled);
}

void TweaksSettingsHomeMenuView::on_help_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showHelp", enabled);
}

void TweaksSettingsHomeMenuView::on_browser_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showBrowser", enabled);
}

void TweaksSettingsHomeMenuView::on_airplaneMode_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showAirplaneMode", enabled);
}

void TweaksSettingsHomeMenuView::on_toggleWifi_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showWifiOnOff", enabled);
}

void TweaksSettingsHomeMenuView::on_powerOff_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/showPowerOff", enabled);
}
