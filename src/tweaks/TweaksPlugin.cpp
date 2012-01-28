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
#include "../../include/N3SettingsController.h"
#include "../../include/N3SyncManager.h"
#include "../../include/WirelessWorkflowManager.h"
#include "../../include/HomeMenuController.h"
#include "../qtscript/QtScriptPlugin.h"


using namespace std;

TweaksPlugin::TweaksPlugin() :
    sw(NULL),
    lastPatchedMenu(NULL)
{
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

            QSettings settings;
            if(settings.value("Tweaks/hideRecommendations", false).toBool()) {
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
    cout << "TweaksPlugin::patchMenu(), ntm: " << ntm << ", hmc: " << hmc << endl << flush; 
    if (hmc && ntm && lastPatchedMenu != (void *) ntm) {
        /* Clear menu and add entries based on configuration
         *
         *
        ntm->clear();
        MenuTextItem *mti = hmc->createMenuTextItem(ntm, QString("Library"), false);
        mti->setSelectedImage(":/images/menu/trilogy_library.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "library", true, true);
        ntm->addSeparator();

        mti = hmc->createMenuTextItem(ntm, QString("Dictionary"), false);
        mti->setSelectedImage(":/images/menu/trilogy_readinglife.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "dictionary", true, true);
        ntm->addSeparator();

        mti = hmc->createMenuTextItem(ntm, QString("Settings"), false);
        mti->setSelectedImage(":/images/menu/trilogy_settings.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "settings", true, true);
        ntm->addSeparator();
        */

        MenuTextItem *mti = hmc->createMenuTextItem(ntm, QString("Tweaks"), false);
        mti->setSelectedImage(":/koboplugins/icons/menu/tweak_01.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-kobo-tweaks", true, true);
        ntm->addSeparator();

        QSettings settings;
        if(settings.value("Tweaks/enableBrowserShortcut", true).toBool()) {
            mti = hmc->createMenuTextItem(ntm, QString("Browser"), false);
	        mti->setSelectedImage(":/koboplugins/icons/menu/browser_01.png");	
            // Hack, since there's no actual way to launch "applications"
            hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-browser", true, true);
            ntm->addSeparator();
        }

        /* Clear menu and add entries based on configuration
         *
         *
        mti = hmc->createMenuTextItem(ntm, QString("Toggle Airplane Mode"), false);
        mti->setSelectedImage(":/images/statusbar/wifi_airplane.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "airplaneMode", true, true);
        ntm->addSeparator();
        */

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
    cout << "TweaksPlugin::enableBrowserShortcut(): " << enable << endl << flush; 
    QSettings settings;
    settings.setValue("Tweaks/enableBrowserShortcut", enable);
}

void TweaksPlugin::enableWirelessTimeout(bool enable)
{
    cout << "TweaksPlugin::enableWirelessTimeout(): " << enable << endl << flush; 
    QSettings settings;
    settings.setValue("Tweaks/enableWirelessTimeout", enable);
}

bool TweaksPlugin::wirelessTimeoutEnabled()
{
    return true;
}

void TweaksPlugin::hideRecommendations(bool enable)
{
    cout << "TweaksPlugin::hideRecommendations(): " << enable << endl << flush; 
    QSettings settings;
    settings.setValue("Tweaks/hideRecommendations", enable);
}

void TweaksPlugin::sync(bool)
{
    cout << "TweaksPlugin::sync()" << endl << flush; 
    N3SyncManager::sharedInstance()->sync();
}

Q_EXPORT_PLUGIN2(tictactoe, TweaksPlugin)
