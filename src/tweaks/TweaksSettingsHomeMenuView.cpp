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

void TweaksSettingsHomeMenuView::on_customMenu_toggled(bool enabled) {
    plugin->settings()->setValue("Menu/customMenuEnabled", enabled);
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
