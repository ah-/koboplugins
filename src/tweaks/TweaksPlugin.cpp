#include "TweaksPlugin.h"
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <iostream>

#include "../../include/PluginInterface.h"
//#include "../../include/QtScriptPluginInterface.h"
#include "../../include/PluginLoader.h"
#include "../../include/MainWindowController.h"
#include "../../include/HomePageGridView.h"
#include "../../include/Content.h"
#include "../../include/Volume.h"
#include "../../include/ReadingViewMixin.h"
#include "../../include/N3ReaderOpener.h"
#include "../../include/N3SettingsController.h"
#include "../../include/N3FSSyncManager.h"
#include "../../include/WirelessWorkflowManager.h"
#include "../../include/HomeMenuController.h"
#include "../../include/WirelessWatchdog.h"
#include "../qtscript/QtScriptPlugin.h"
#include "config.h"

using namespace std;

TweaksPlugin::TweaksPlugin() :
    sw(NULL),
    lastPatchedMenu(NULL)
{
    // check if koboplugins.ini exists otherwise extract resource
    QFile f("/mnt/onboard/.kobo/koboplugins.ini");
    if(!f.exists())
        QFile::copy(":/koboplugins/templates/koboplugins.ini", "/mnt/onboard/.kobo/koboplugins.ini");

    PluginsConfig::init("/mnt/onboard/.kobo/koboplugins.ini");

    if(!checkFirmwareVersion())
        return;

    QTranslator* pTranslator = new QTranslator;
    QString locale = QLocale::system().name();
    pTranslator->load(QString("koboplugins_") + PluginsConfig::get()->value("Global/language", "en").toString(), ":/koboplugins/translations/");
    qApp->installTranslator(pTranslator);

    cout << "TweaksPlugin()" << endl << flush; 

    // try to register with the qtscript plugin
    // TODO: use interface, proper casting
    QtScriptPlugin *qtscriptPlugin = (QtScriptPlugin *) PluginLoader::forMimeType("application/x-qt-script");
    if (qtscriptPlugin) {
        cout << "found qtscript plugin: " << qtscriptPlugin << endl << flush;
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
    enableWirelessTimeout(PluginsConfig::get()->value("Tweaks/enableWirelessTimeout", false).toBool());
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

QWidget *TweaksPlugin::reader(void* plugin_state, QWidget *parent)
{
    cout << "TweaksPlugin::reader()" << endl << flush; 

    w = new TweaksWidget(this, parent);
    QObject::connect(w, SIGNAL(clicked(void)), w, SLOT(onPush(void)));
    w->setGeometry(0, 0, 600, 734);
    w->update();
	return w;
}

ParserInterface *TweaksPlugin::parser()
{
    return NULL;
}

void TweaksPlugin::windowChanged(int index)
{
	//static QPushButton* b = NULL;

    cout << "TweaksPlugin::windowChanged()" << endl << flush; 

    QWidget *currentWidget = const_cast<QWidget *>(sw->widget(index));
    if (currentWidget) {
        cout << "TweaksPlugin::windowChanged(), current Widget: " << currentWidget->metaObject()->className();
        cout << " at " << currentWidget << endl << flush; 

        HomePageGridView *hpgv = qobject_cast<HomePageGridView *>(currentWidget);
        if (hpgv) {
            cout << "TweaksPlugin::windowChanged() found HomePageGridView, trying to patch menu" << endl << flush; 

            HomePageGridView *hpgv = qobject_cast<HomePageGridView *>(currentWidget);
            HomePageGridViewHeader *hpgvh = hpgv->findChild<HomePageGridViewHeader *>("header");
            TextHeader *home = NULL;
            if (hpgvh)
                home = hpgvh->findChild<TextHeader *>();

            cout << "TweaksPlugin::windowChanged(), hpgvh: " << hpgvh << ", home: " << home << endl << flush; 

            if (home) {
                // patch menu when it's displayed for the first time
                connect(home, SIGNAL(mouseDown()), this, SLOT(patchMenu()), (Qt::ConnectionType) 0);
            }

			/*
			 Hide START menu and display Buttons instead of menu
		     So far the menu hide works --> button is not yet displayed
			 
			ReversibleLabel *pLabel = hpgv->findChild<ReversibleLabel *>();
		
			QLayout* parent = pLabel->layout();
			if(parent)
				{
				QBoxLayout* bL = dynamic_cast<QBoxLayout*>(parent);
				if(bL)
					{
					int menuIdx = bL->indexOf(pLabel);
					pLabel->hide();
					if(!b)
						{
						b = new QPushButton("Testbtn");
						b->setMinimumWidth(100);
						b->setMinimumHeight(40);
						bL->insertWidget(menuIdx, b);
						}
					b->show();
					}
				}
			*/

            PluginsConfig* pConfig = PluginsConfig::get();
            if(pConfig->value("Tweaks/hideRecommendations", false).toBool()) {
                HomePageGridViewFooter *footer = hpgv->findChild<HomePageGridViewFooter *>("footer");
                footer->hide();
           	}

            if(pConfig->value("Tweaks/hideSyncIcon", false).toBool()) {
		        TouchLabel *syncIcon = qApp->activeWindow()->findChild<TouchLabel *>("syncIcon");
				if(syncIcon)
		        	syncIcon->hide();
			}
        }
    }
}

void TweaksPlugin::patchMenu()
{
    cout << "TweaksPlugin::patchMenu()" << endl << flush; 
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();
    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();
    PluginsConfig* pConfig = PluginsConfig::get();

    cout << "TweaksPlugin::patchMenu(), ntm: " << ntm << ", hmc: " << hmc << endl << flush; 
    if (hmc && ntm && lastPatchedMenu != (void *) ntm) {
        // Clear menu and add entries based on configuration
        MenuTextItem *mti = NULL;

        if(pConfig->value("Menu/customMenuEnabled", false).toBool()) {
            ntm->clear();

            if(pConfig->value("Menu/showLibrary", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Library")), false);
                mti->setSelectedImage(":/images/menu/trilogy_library.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "library", true, true);
                ntm->addSeparator();
            }

            if(lmc && pConfig->value("Menu/showShortlist", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Shortlist")), false);
                mti->setSelectedImage(":/koboplugins/icons/menu/shortlist_01.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "shortlist", true, true);
                ntm->addSeparator();
            }

            if(lmc && pConfig->value("Menu/showSearch", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Library Search")), false);
                mti->setSelectedImage(":/koboplugins/icons/menu/search_01.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "search", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showDictionary", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Dictionary")), false);
                mti->setSelectedImage(":/koboplugins/icons/menu/dictionary_01.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "dictionary", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showReadingLife", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Reading Life")), false);
                mti->setSelectedImage(":/images/menu/trilogy_readinglife.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "readingLife", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showStore", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Store")), false);
                mti->setSelectedImage(":/images/menu/trilogy_store.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "store", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showSync", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Sync")), false);
                mti->setSelectedImage(":/images/menu/trilogy_sync.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "sync", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showHelp", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString(tr("Help")), false);
                mti->setSelectedImage(":/images/menu/trilogy_help.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "help", true, true);
                ntm->addSeparator();
            }

            mti = hmc->createMenuTextItem(ntm, QString(tr("Settings")), false);
            mti->setSelectedImage(":/images/menu/trilogy_settings.png");
            mti->setSelected(true);
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "settings", true, true);
            ntm->addSeparator();
            }

        mti = hmc->createMenuTextItem(ntm, QString(tr("Tweaks")), false);
        mti->setSelectedImage(":/koboplugins/icons/menu/tweak_01.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-kobo-tweaks", true, true);
        ntm->addSeparator();

        if(pConfig->value("Menu/showBrowser", false).toBool()) {
            mti = hmc->createMenuTextItem(ntm, QString(tr("Browser")), false);
	        mti->setSelectedImage(":/koboplugins/icons/menu/browser_01.png");	
			mti->setSelected(true);
            // Hack, since there's no actual way to launch "applications"
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-browser", true, true);
            ntm->addSeparator();
            }

        if(pConfig->value("Menu/showAirplaneMode", false).toBool()) {
            mti = hmc->createMenuTextItem(ntm, QString(tr("Toggle WiFi")), false);
            mti->setSelectedImage(":/images/statusbar/wifi_airplane.png");
            mti->setSelected(true);
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "airplaneMode", true, true);
            ntm->addSeparator();
            }

        // store that we already patched this menu so the item doesn't get added twice
        lastPatchedMenu = (void *) ntm;
        cout << "TweaksPlugin::patchMenu(), success!" << endl << flush; 
    }
}

void TweaksPlugin::open(QString mimeType)
{
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();

    cout << "TweaksPlugin::open(\"" << mimeType.toStdString() << "\")" << endl << flush; 
    if (mimeType == "application/x-browser") {
        WirelessWorkflowManager::sharedInstance()->openBrowser(QUrl());
    } 
	else if (mimeType == "library") {
		if(hmc)
			hmc->library();
	}
	else if (mimeType == "dictionary") {
		if(hmc)
			hmc->dictionary();
	}
	else if (mimeType == "settings") {
		if(hmc)
			hmc->settings();
	}
    else if (mimeType == "readingLife") {
        if(hmc)
            hmc->readingLife();
    }
    else if (mimeType == "store") {
        if(hmc)
            hmc->store();
    }
    else if (mimeType == "sync") {
        if(hmc)
            hmc->sync();
    }
    else if (mimeType == "help") {
        if(hmc)
            hmc->help();
    }
	else if (mimeType == "airplaneMode") {
        N3SettingsWirelessController* p = QApplication::activeWindow()->findChild<N3SettingsWirelessController *>();    
        if(p)
            p->airplaneModeToggled();
	}
	else if (mimeType == "shortlist") {
	    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
        if(lmc)
            lmc->favourites();
	    }
	else if (mimeType == "search") {
	    LibraryMenuController *lmc = QApplication::activeWindow()->findChild<LibraryMenuController *>();
        if(lmc)
            lmc->search();
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
    PluginsConfig* pConfig = PluginsConfig::get();

    cout << "TweaksPlugin::enableBrowserShortcut(): " << enable << endl << flush; 
    pConfig->setValue("Menu/showBrowser", enable);
}

void TweaksPlugin::enableWirelessTimeout(bool enable)
{
    cout << "TweaksPlugin::enableWirelessTimeout(): " << enable << endl << flush; 
    PluginsConfig::get()->setValue("Tweaks/enableWirelessTimeout", enable);

    WirelessWatchdog *wd = WirelessWatchdog::sharedInstance();
    cout << "TweaksPlugin::enableWirelessTimeout(): " << wd << endl << flush; 
    if(wd) {
        wd->setEnabled(enable);
    }
}

bool TweaksPlugin::wirelessTimeoutEnabled()
{
    PluginsConfig* pConfig = PluginsConfig::get();
    return pConfig->value("Tweaks/enableWirelessTimeout", true).toBool();
}

void TweaksPlugin::hideRecommendations(bool enable)
{
    cout << "TweaksPlugin::hideRecommendations(): " << enable << endl << flush; 
    PluginsConfig* pConfig = PluginsConfig::get();
    pConfig->setValue("Tweaks/hideRecommendations", enable);
}

void TweaksPlugin::uninstallPlugin()
{
	QFile::remove("/usr/local/Kobo/libtweaks.so");
}

void TweaksPlugin::sync(bool)
{
    cout << "TweaksPlugin::sync()" << endl << flush; 
    QStringList paths;
    paths << "/mnt/onboard/";
    N3FSSyncManager::sharedInstance()->sync(paths);
}

bool TweaksPlugin::checkFirmwareVersion()
{
    PluginsConfig* pConfig = PluginsConfig::get();

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
    if(versionParts[2] != pConfig->value("Global/compatFirmware", "1.9.16").toString())
        return false;

    return true;
}

Q_EXPORT_PLUGIN2(tictactoe, TweaksPlugin)
