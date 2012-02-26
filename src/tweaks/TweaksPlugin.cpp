#include "TweaksPlugin.h"

#include <iostream>

#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

#include "../qtscript/QtScriptPlugin.h"
#include "PluginInterface.h"
#include "PluginLoader.h"

#include "Content.h"
#include "DevicePowerWorkflowManager.h"
#include "GridAndListLibraryView.h"
#include "HomeMenuController.h"
#include "HomePageGridView.h"
#include "MainWindowController.h"
#include "N3FSSyncManager.h"
#include "N3ReaderOpener.h"
#include "N3SettingsController.h"
#include "ReadingView.h"
#include "ReadingViewMixin.h"
#include "TouchLabel.h"
#include "TweaksSettingsPageView.h"
#include "TweaksSettingsView.h"
#include "Volume.h"
#include "WirelessWatchdog.h"
#include "WirelessWorkflowManager.h"

#include "menudefines.h"

using namespace std;

TweaksPlugin::TweaksPlugin() :
    pluginSettings(new QSettings(CONFIG_PATH, QSettings::IniFormat)),
    sw(NULL),
    lastPatchedMenu(NULL),
    lastPatchedLibraryMenu(NULL)
{
    // init logging engine
    //m_pLogBase = LoggerBase::get();
    //FileLogger* pLoggerFile = new FileLogger("/mnt/onboard/", true);
    //m_pLogBase->addLogObject("FileLogger", pLoggerFile);
    //m_pLogBase->start();

    //LOG("checking firmware version");

    if(!checkFirmwareVersion())
        return;

    //LOG("init translator");
    QTranslator* pTranslator = new QTranslator;
    pTranslator->load(QString("koboplugins.") + QLocale().name(), ":/koboplugins/translations/");
    qApp->installTranslator(pTranslator);

    //LOG("TweaksPlugin()");

    // try to register with the qtscript plugin
    // TODO: use interface, proper casting
    QtScriptPlugin *qtscriptPlugin = (QtScriptPlugin *) PluginLoader::forMimeType("application/x-qt-script");
    if (qtscriptPlugin) {
        //LOG("found qtscript plugin: 0x%x>",qtscriptPlugin);
        qtscriptPlugin->registerObject(this, "TweaksPlugin");
    }

    // register to get notified when the current main window changes
    MainWindowController *mwc = MainWindowController::sharedInstance();
    if (mwc) {
        cout << "found MainWindowController: " << mwc << endl << flush;
        sw = QApplication::activeWindow()->findChild<QStackedWidget *>();
        if (sw) {
            cout << "found QStackedWidget: " << sw << endl << flush;
            connect(sw, SIGNAL(currentChanged(int)), this, SLOT(windowChanged(int)));
            cout << "connected windowChanged signal" << endl << flush;
        }
    }

    connect(&mapper, SIGNAL(mapped(QString)), this, SLOT(open(QString)));
    enableWirelessTimeout(pluginSettings->value("Tweaks/enableWirelessTimeout", false).toBool());
}

TweaksPlugin::~TweaksPlugin()
{
    cout << "~TweaksPlugin()" << endl << flush; 
}

QStringList TweaksPlugin::mimeTypes()
{
    cout << "TweaksPlugin::mimeTypes()" << endl << flush; 

    QStringList mimeTypes;
    mimeTypes << "application/x-kobo-tweaks";
    return mimeTypes;
}

QWidget *TweaksPlugin::reader(void* /*plugin_state*/, QWidget * /*parent*/)
{
    cout << "TweaksPlugin::reader()" << endl << flush; 
    return NULL;
}

ParserInterface *TweaksPlugin::parser()
{
    cout << "TweaksPlugin::parser()" << endl << flush; 
    return NULL;
}

void TweaksPlugin::windowChanged(int index)
{
    cout << "TweaksPlugin::windowChanged()" << endl << flush; 

    QWidget *currentWidget = const_cast<QWidget *>(sw->widget(index));
    if (currentWidget) {
        cout << "TweaksPlugin::windowChanged(), current Widget: " << currentWidget->metaObject()->className();
        cout << " at " << currentWidget << endl << flush; 

        HomePageGridView *hpgv = qobject_cast<HomePageGridView *>(currentWidget);
        GridAndListLibraryView *gallv = qobject_cast<GridAndListLibraryView *>(currentWidget);
        ReadingView *readv = qobject_cast<ReadingView *>(currentWidget);

        if(readv) {
            if(pluginSettings->value("Reader/tweakFooter", false).toBool())
                connect(readv, SIGNAL(footerMenuOpened()), this, SLOT(bookFooterOpened()));
        }
        else if (hpgv) {
            cout << "TweaksPlugin::windowChanged() found HomePageGridView, trying to patch menu" << endl << flush; 

            HomePageGridViewHeader *hpgvh = hpgv->findChild<HomePageGridViewHeader *>("header");
            TextHeader *home = NULL;
            if (hpgvh)
                home = hpgvh->findChild<TextHeader *>();

            cout << "TweaksPlugin::windowChanged(), hpgvh: " << hpgvh << ", home: " << home << endl << flush; 

            if (home) {
                // patch menu when it's displayed for the first time
                connect(home, SIGNAL(mouseDown()), this, SLOT(patchMenu()), (Qt::ConnectionType) 0);
            }

            if(pluginSettings->value("Tweaks/hideRecommendations", false).toBool()) {
                HomePageGridViewFooter *footer = hpgv->findChild<HomePageGridViewFooter *>("footer");
                footer->hide();
            }

            if(pluginSettings->value("Tweaks/hideSyncIcon", false).toBool()) {
                TouchLabel *syncIcon = qApp->activeWindow()->findChild<TouchLabel *>("syncIcon");
                if(syncIcon)
                    syncIcon->hide();
            }
        }
        else if (gallv) {
            cout << "TweaksPlugin::windowChanged() found HomePageGridView, trying to patch menu" << endl << flush;

            LibraryViewHeader *gallvh = gallv->findChild<LibraryViewHeader *>("header");
            TextHeader *home = NULL;
            if (gallvh)
                home = gallvh->findChild<TextHeader *>("libraryLabel");

            if (home) {
                connect(home, SIGNAL(mouseDown()), this, SLOT(patchLibraryMenu()), (Qt::ConnectionType) 0);
            }
        }
    }
}

void TweaksPlugin::patchMenu()
{
    //LOG("patchMenu");

    cout << "TweaksPlugin::patchMenu()" << endl << flush; 
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();
    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();

    cout << "TweaksPlugin::patchMenu(), ntm: " << ntm << ", hmc: " << hmc << endl << flush; 
    if (hmc && ntm && lastPatchedMenu != (void *) ntm) {
        // Clear menu and add entries based on configuration

        if(pluginSettings->value("Menu/customMenuEnabled", true).toBool()) {
            ntm->clear();

            if(pluginSettings->value("Menu/showLibrary", true).toBool())
                createHomeMenuEntry(MENTRY_LIBRARY, ":/images/menu/trilogy_library.png", tr("Library"));

            if(lmc && pluginSettings->value("Menu/showShortlist", false).toBool())
                createHomeMenuEntry(MENTRY_SHORTLIST, ":/koboplugins/icons/menu/shortlist_01.png", tr("Shortlist"));

            if(lmc && pluginSettings->value("Menu/showShelves", false).toBool())
                createHomeMenuEntry(MENTRY_SHELVES, ":/koboplugins/icons/menu/shelve_01.png", tr("Bookshelves"));

            if(lmc && pluginSettings->value("Menu/showSearch", true).toBool())
                createHomeMenuEntry(MENTRY_LIBRARYSEARCH, ":/koboplugins/icons/menu/search_02.png", tr("Library Search"));

            if(pluginSettings->value("Menu/showDictionary", true).toBool())
                createHomeMenuEntry(MENTRY_DICTIONARY, ":/koboplugins/icons/menu/dictionary_01.png", tr("Dictionary"));

            if(pluginSettings->value("Menu/showReadingLife", true).toBool())
                createHomeMenuEntry(MENTRY_READINGLIFE, ":/images/menu/trilogy_readinglife.png", tr("Reading Life"));

            if(pluginSettings->value("Menu/showStore", true).toBool())
                createHomeMenuEntry(MENTRY_STORE, ":/images/menu/trilogy_store.png", tr("Store"));

            if(pluginSettings->value("Menu/showSync", true).toBool())
                createHomeMenuEntry(MENTRY_SYNC, ":/images/menu/trilogy_sync.png", tr("Sync"));

            if(pluginSettings->value("Menu/showHelp", true).toBool())
                createHomeMenuEntry(MENTRY_HELP, ":/images/menu/trilogy_help.png", tr("Help"));

            createHomeMenuEntry(MENTRY_SETTINGS, ":/images/menu/trilogy_settings.png", tr("Settings"));
        } else {
            if(lmc && pluginSettings->value("Menu/showShortlist", false).toBool())
                createHomeMenuEntry(MENTRY_SHORTLIST, ":/koboplugins/icons/menu/shortlist_01.png", tr("Shortlist"));
        }

        if(pluginSettings->value("Menu/showTweaksEntry", true).toBool())
            createHomeMenuEntry(MENTRY_TWEAKS, ":/koboplugins/icons/menu/tweak_01.png", tr("Tweaks"));

        if(pluginSettings->value("Menu/showBrowser", false).toBool())
            createHomeMenuEntry(MENTRY_BROWSER, ":/koboplugins/icons/menu/browser_02.png", tr("Browser"));

        if(pluginSettings->value("Menu/showAirplaneMode", false).toBool())
            createHomeMenuEntry(MENTRY_AIRPLANEMODE, ":/images/statusbar/wifi_airplane.png", tr("Airplane Mode"));

        if(pluginSettings->value("Menu/showWifiOnOff", false).toBool())
            createHomeMenuEntry(MENTRY_WIFIONOFF, ":/images/statusbar/wifi_4.png", tr("Toggle WiFi"));

        if(pluginSettings->value("Menu/showPowerOff", false).toBool())
            createHomeMenuEntry(MENTRY_POWEROFF, ":/koboplugins/icons/menu/power_01.png", tr("Power Off"));

        // store that we already patched this menu so the item doesn't get added twice
        lastPatchedMenu = (void *) ntm;
        cout << "TweaksPlugin::patchMenu(), success!" << endl << flush; 
    }
}

void TweaksPlugin::patchLibraryMenu()
{
    cout << "TweaksPlugin::patchMenu()" << endl << flush;
    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();

    if (lmc && ntm && lastPatchedLibraryMenu != (void *) ntm) {
        // Clear menu and add entries based on configuration

        if(pluginSettings->value("Library/customMenuEnabled", false).toBool()) {
            ntm->clear();

            if(pluginSettings->value("Library/showBooks", true).toBool())
                createLibraryMenuEntry(MENTRY_BOOKS, tr("Books"));

            if(pluginSettings->value("Library/showSortlist", true).toBool())
                createLibraryMenuEntry(MENTRY_SHORTLIST, tr("Shortlist"));

            if(pluginSettings->value("Library/showSearch", true).toBool())
                createLibraryMenuEntry(MENTRY_LIBRARYSEARCH, tr("Library Search"));
        }

        if(pluginSettings->value("Library/showShelves", true).toBool()) {
            createLibraryMenuEntry(MENTRY_CREATESHELF, tr("Create Shelf"));
            createLibraryMenuEntry(MENTRY_SHELVES, tr("Bookshelves"));
        }

        // store that we already patched this menu so the item doesn't get added twice
        lastPatchedLibraryMenu = (void *) ntm;
        cout << "TweaksPlugin::patchMenu(), success!" << endl << flush;
    }
}

void TweaksPlugin::open(QString mimeType)
{
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();
    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();

    cout << "TweaksPlugin::open(\"" << mimeType.toStdString() << "\")" << endl << flush; 
    if (mimeType == MENTRY_BROWSER) {
        WirelessWorkflowManager::sharedInstance()->openBrowser(QUrl());
    } 
    else if (mimeType == MENTRY_TWEAKS) {
        // TODO: proper parent/lifecycle management
        TweaksSettingsView *settingsView = new TweaksSettingsView(this, 0);
        TweaksSettingsPageView *v = new TweaksSettingsPageView(QApplication::activeWindow(), settingsView, false);
        MainWindowController::sharedInstance()->pushView(v);
    }
    else if (mimeType == MENTRY_WIFIONOFF) {
        WirelessWorkflowManager* wfm = WirelessWorkflowManager::sharedInstance();
        if(!wfm->isAirplaneMode()) {
            wfm->turnWifiOff();
            wfm->setAirplaneMode(true);
        } else {
            wfm->setAirplaneMode(false);
            wfm->connectWirelessSilently();
            enableWirelessTimeout(wirelessTimeoutEnabled());
        }
    }
    else if (mimeType == MENTRY_LIBRARY) {
        if(hmc)
            hmc->library();
    }
    else if (mimeType == MENTRY_DICTIONARY) {
        if(hmc)
            hmc->dictionary();
    }
    else if (mimeType == MENTRY_SETTINGS) {
        if(hmc)
            hmc->settings();
    }
    else if (mimeType == MENTRY_READINGLIFE) {
        if(hmc)
            hmc->readingLife();
    }
    else if (mimeType == MENTRY_STORE) {
        if(hmc)
            hmc->store();
    }
    else if (mimeType == MENTRY_SYNC) {
        if(hmc)
            hmc->sync();
    }
    else if (mimeType == MENTRY_HELP) {
        if(hmc)
            hmc->help();
    }
    else if (mimeType == MENTRY_AIRPLANEMODE) {
        N3SettingsWirelessController* p = QApplication::activeWindow()->findChild<N3SettingsWirelessController *>();    
        if(p)
            p->airplaneModeToggled();
    }
    else if (mimeType == MENTRY_SHORTLIST) {
        if(lmc)
            lmc->favourites();
    }
    else if (mimeType == MENTRY_BOOKS) {
        if(lmc)
            lmc->lists();
    }
    else if (mimeType == MENTRY_CREATESHELF) {
        if(lmc)
            lmc->createShelf();
    }
    else if (mimeType == MENTRY_SHELVES) {
        if(lmc)
            lmc->shelves();
    }
    else if (mimeType == MENTRY_LIBRARYSEARCH) {
        if(lmc)
            lmc->search();
    }
    else if (mimeType == MENTRY_POWEROFF) {
        N3PowerWorkflowManager::sharedInstance()->showPowerOffView();
        DevicePowerWorkflowManager* p = qApp->findChild<DevicePowerWorkflowManager *>();    
        if(p)
            p->powerOff(false);
    }
    else {
        Volume v;
        v.setMimeType(mimeType);
        v.setTitle("Tweaks");
        N3ReaderOpener *ro = (N3ReaderOpener*) ((ReadingViewMixin*) this)->createReader(v, NULL);
        ro->openReader();
    }
}

void TweaksPlugin::enableBrowserShortcut(bool enable)
{
    cout << "TweaksPlugin::enableBrowserShortcut(): " << enable << endl << flush; 
    pluginSettings->setValue("Menu/showBrowser", enable);
}

void TweaksPlugin::enableWirelessTimeout(bool enable)
{
    cout << "TweaksPlugin::enableWirelessTimeout(): " << enable << endl << flush; 
    pluginSettings->setValue("Tweaks/enableWirelessTimeout", enable);

    WirelessWatchdog::sharedInstance()->setEnabled(enable);
}

bool TweaksPlugin::wirelessTimeoutEnabled()
{
    return pluginSettings->value("Tweaks/enableWirelessTimeout", true).toBool();
}

void TweaksPlugin::hideRecommendations(bool enable)
{
    cout << "TweaksPlugin::hideRecommendations(): " << enable << endl << flush; 
    pluginSettings->setValue("Tweaks/hideRecommendations", enable);
}

void TweaksPlugin::enableShelves(bool enable)
{
    cout << "TweaksPlugin::enableShelves(): " << enable << endl << flush; 
    pluginSettings->setValue("Menu/showShelves", enable);
}

void TweaksPlugin::uninstallPlugin()
{
    cout << "TweaksPlugin::uninstallPlugin()" << endl << flush; 
    QFile::remove("/usr/local/Kobo/libtweaks.so");
    QFile::remove("/mnt/onboard/.kobo/koboplugins.ini");
    system("/bin/busybox reboot");
}

void TweaksPlugin::sync(bool)
{
    cout << "TweaksPlugin::sync()" << endl << flush; 
    QStringList paths;
    paths << "/mnt/onboard/";
    N3FSSyncManager::sharedInstance()->sync(paths);
}

void TweaksPlugin::openBrowser()
{
    WirelessWorkflowManager::sharedInstance()->openBrowser(QUrl());
}

void TweaksPlugin::bookFooterOpened()
{
    if(pluginSettings->value("Reader/showBrowser", false).toBool()) {
        // add browser icon before dict label
        if(!qApp->activeWindow()->findChild<TouchLabel *>("readerBrowserLabel")) {
            TouchLabel *dict = qApp->activeWindow()->findChild<TouchLabel *>("dict");
            if(dict) {
                QList<QHBoxLayout*> layouts = qApp->activeWindow()->findChildren<QHBoxLayout *>("horizontalLayout_2");
                QHBoxLayout* parent = NULL;
                if(layouts.size() >= 4)
                    parent = layouts.at(3);

                if(parent) {
                    TouchLabel *browser = new TouchLabel("Browser", qApp->activeWindow());
                    if(browser) {
                        browser->setObjectName("readerBrowserLabel");
                        //                    browser->setSelectedPixmap(":/koboplugins/icons/menu/browser_02.png");
                        //                    browser->setDeselectedPixmap(":/koboplugins/icons/menu/browser_02.png");
                        connect(browser, SIGNAL(tapped()), this, SLOT(openBrowser()));
                        parent->addWidget(browser);
                        browser->show();
                    }
                }
            }
        }
    } else {
        if(qApp->activeWindow()->findChild<TouchLabel *>("readerBrowserLabel")) {
            TouchLabel *browser = qApp->activeWindow()->findChild<TouchLabel *>("readerBrowserLabel");
            if(browser)
                browser->hide();
        }
    }
}

bool TweaksPlugin::checkFirmwareVersion()
{
    QString requiredVersion = pluginSettings->value("Global/compatFirmware", "1.9.16").toString();
    if(requiredVersion == "0.0.0")
        return true;	

    // check if firmware version matches 1.9.16
    QFile f("/mnt/onboard/.kobo/version");
    if(!f.open(QIODevice::ReadOnly))
        return false;

    char szVersion[100];
    f.readLine(szVersion, 99);
    QString versionString(szVersion);
    QStringList versionParts = versionString.split(",");
    if(versionParts.size() != 6)
        return false;

    // now check fw version
    if(versionParts[2] != requiredVersion)
        return false;

    return true;
}

// TODO: don't call findChild all the time
bool TweaksPlugin::createHomeMenuEntry(QString mapping, QString icon, QString text)
{
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();
    MenuTextItem *mti = NULL;

    mti = hmc->createMenuTextItem(ntm, text, false);
    if(!icon.isEmpty())
        mti->setSelectedImage(icon);

    mti->setSelected(true);
    hmc->addWidgetActionWithMapper(ntm, mti, &mapper, mapping, true, true);
    ntm->addSeparator();
    return true;
}

bool TweaksPlugin::createLibraryMenuEntry(QString mapping,QString text)
{
    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();
    MenuTextItem *mti = NULL;

    mti = lmc->createMenuTextItem(ntm, text, false);
    lmc->addWidgetActionWithMapper(ntm, mti, &mapper, mapping, true, true);
    ntm->addSeparator();
    return true;
}

QSharedPointer<QSettings> TweaksPlugin::settings() {
    return pluginSettings;
}

Q_EXPORT_PLUGIN2(tictactoe, TweaksPlugin)
