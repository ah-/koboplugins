#include "TweaksPlugin.h"
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
#include "../../include/WirelessWorkflowManager.h"
#include "../../include/HomeMenuController.h"
#include "../../include/N3SettingsController.h"
#include "../qtscript/QtScriptPlugin.h"
#include "config.h"

using namespace std;

TweaksPlugin::TweaksPlugin() :
    sw(NULL),
    lastPatchedMenu(NULL)
{
    PluginsConfig::init("/mnt/onboard/.kobo/koboplugins.ini");

    if(!checkFirmwareVersion())
        return;

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
    //enableWirelessTimeout(settings.value("Tweaks/enableWirelessTimeout", false).toBool());
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
	static QPushButton* b = NULL;

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
                connect(home, SIGNAL(mouseDown()), this, SLOT(patchMenu()),(Qt::ConnectionType)  0);
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
        }
    }
}

void TweaksPlugin::patchMenu()
{
    cout << "TweaksPlugin::patchMenu()" << endl << flush; 
    HomeMenuController *hmc = QApplication::activeWindow()->findChild<HomeMenuController *>();
    NickelTouchMenu *ntm = QApplication::activeWindow()->findChild<NickelTouchMenu *>();
    PluginsConfig* pConfig = PluginsConfig::get();

    cout << "TweaksPlugin::patchMenu(), ntm: " << ntm << ", hmc: " << hmc << endl << flush; 
    if (hmc && ntm && lastPatchedMenu != (void *) ntm) {
        /* Clear menu and add entries based on configuration
         *
         */
        MenuTextItem *mti = NULL;

        if(pConfig->value("Menu/customMenuEnabled", false).toBool()) {
            ntm->clear();

            if(pConfig->value("Menu/showLibrary", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Library"), false);
                mti->setSelectedImage(":/images/menu/trilogy_library.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "library", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showDictionary", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Dictionary"), false);
                mti->setSelectedImage(":/images/menu/trilogy_readinglife.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "dictionary", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showReadingLife", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Reading Life"), false);
                mti->setSelectedImage(":/images/menu/trilogy_readinglife.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "readingLife", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showStore", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Store"), false);
                mti->setSelectedImage(":/images/menu/trilogy_store.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "store", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showSync", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Sync"), false);
                mti->setSelectedImage(":/images/menu/trilogy_sync.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "sync", true, true);
                ntm->addSeparator();
            }

            if(pConfig->value("Menu/showHelp", true).toBool()) {
                mti = hmc->createMenuTextItem(ntm, QString("Help"), false);
                mti->setSelectedImage(":/images/menu/trilogy_help.png");
                mti->setSelected(true);
                hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "help", true, true);
                ntm->addSeparator();
            }

            mti = hmc->createMenuTextItem(ntm, QString("Settings"), false);
            mti->setSelectedImage(":/images/menu/trilogy_settings.png");
            mti->setSelected(true);
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "settings", true, true);
            ntm->addSeparator();
            }

        mti = hmc->createMenuTextItem(ntm, QString("Tweaks"), false);
        mti->setSelectedImage(":/koboplugins/icons/menu/tweak_01.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-kobo-tweaks", true, true);
        ntm->addSeparator();

        if(pConfig->value("Menu/showBrowser", false).toBool()) {
            mti = hmc->createMenuTextItem(ntm, QString("Browser"), false);
	        mti->setSelectedImage(":/koboplugins/icons/menu/browser_01.png");	
			mti->setSelected(true);
            // Hack, since there's no actual way to launch "applications"
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-browser", true, true);
            ntm->addSeparator();
            }

        if(pConfig->value("Menu/showAirplaneMode", false).toBool()) {
            mti = hmc->createMenuTextItem(ntm, QString("Toggle Airplane Mode"), false);
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
    PluginsConfig* pConfig = PluginsConfig::get();
    pConfig->setValue("Tweaks/enableWirelessTimeout", enable);
}

bool TweaksPlugin::wirelessTimeoutEnabled()
{
    return true;
}

void TweaksPlugin::hideRecommendations(bool enable)
{
    cout << "TweaksPlugin::hideRecommendations(): " << enable << endl << flush; 
    PluginsConfig* pConfig = PluginsConfig::get();
    pConfig->setValue("Tweaks/hideRecommendations", enable);
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
