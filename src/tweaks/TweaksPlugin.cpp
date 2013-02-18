#include "TweaksPlugin.h"

#include <iostream>

#include <QFile>
#include <QLocale>
#include <QTranslator>

#include "../qtscript/QtScriptPlugin.h"
#include "PluginLoader.h"

#include "HomePageGridView.h"
#include "MainWindowController.h"
#include "N3FSSyncManager.h"
#include "N3ReaderOpener.h"
#include "N3SettingsController.h"
#include "ReadingView.h"
#include "TouchLabel.h"
#include "TweaksSettingsPageView.h"
#include "TweaksSettingsView.h"
#include "FooterLabelWidget.h"
#include "ReadingLifeMenuController.h"
#include "N3PowerWorkflowManager.h"
#include "LibraryMenuController.h"
#include "WirelessWorkflowManager.h"

#include "cpplog.hpp"

#define REQUIRED_VERSION "2.4.0"

using namespace cpplog;
using namespace std;

StdErrLogger l;

TweaksPlugin::TweaksPlugin(QObject * /*parent*/) :
    pluginSettings(new QSettings(CONFIG_PATH, QSettings::IniFormat))
{
    menuEntries << MenuEntry("Browser", "tweaks.openBrowser()", true);
    menuEntries << MenuEntry("Sudoku", "tweaks.sudoku()", false);
    menuEntries << MenuEntry("Chess", "tweaks.chess()", true);
    menuEntries << MenuEntry("Scribble", "tweaks.scribble()", false);
    menuEntries << MenuEntry("Tweaks Settings", "tweaks.tweaksSettings()", true);
    menuEntries << MenuEntry("Toggle Airplane Mode", "tweaks.toggleAirplaneMode()", false);
    menuEntries << MenuEntry("Sync", "tweaks.sync(true)", false);
    menuEntries << MenuEntry("Sleep", "tweaks.sleep()", false);
    menuEntries << MenuEntry("Power Off", "tweaks.powerOff()", false);

    LOG_TRACE(l);
    if(!checkFirmwareVersion())
        return;

    // Since Qt load imageio plugins before the whole application is started, just register a SLOT to be called when the main window is shown
    connect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(focusChanged(QWidget*, QWidget*)));
}

TweaksPlugin::~TweaksPlugin()
{
    LOG_TRACE(l);
}

QImageIOPlugin::Capabilities TweaksPlugin::capabilities(QIODevice * /*device*/, const QByteArray & /*format*/) const
{
    return QImageIOPlugin::CanRead;
}

QImageIOHandler* TweaksPlugin::create(QIODevice * /*device*/, const QByteArray & /*format*/) const
{
    LOG_TRACE(l);
    return NULL;
}

QStringList TweaksPlugin::keys() const
{
    LOG_TRACE(l);
    // Pretend we can read some nonexistent image format, otherwise Qt unloads the plugin
    QStringList imageFormats;
    imageFormats << "nothing";
    return imageFormats;
}

void TweaksPlugin::focusChanged(QWidget *old, QWidget *now)
{
    LOG_TRACE(l);
    LOG_DEBUG(l) << "old: " << (old ? old->metaObject()->className() : "NULL") << 
                   " now: " << (now ? now->metaObject()->className() : "NULL");

    stackedWidget = QApplication::activeWindow()->findChild<QStackedWidget *>();
    if (stackedWidget) {
        LOG_DEBUG(l) << "found QStackedWidget: " << stackedWidget;

        initialize();

        // disconnect this SLOT since its job is done
        disconnect(QApplication::instance(), SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(focusChanged(QWidget*, QWidget*)));
    }
}

void TweaksPlugin::initialize()
{
    // TODO: set loglevel

    LOG_DEBUG(l) << "init translator";
    LOG_DEBUG(l) << "QLocale().name(): " << QLocale().name().toStdString();
    QTranslator* pTranslator = new QTranslator;
    pTranslator->load(QString("koboplugins.") + QLocale().name(), ":/koboplugins/translations/");
    qApp->installTranslator(pTranslator);

    // try to register with the qtscript plugin
    // TODO: use interface, proper casting
    QtScriptPlugin *qtScriptPlugin = (QtScriptPlugin *) PluginLoader::forMimeType("application/x-qt-script");
    if (qtScriptPlugin) {
        LOG_DEBUG(l) << "found QtScriptPlugin: " << qtScriptPlugin;
        qtScriptPlugin->registerObject(this, "TweaksPlugin");
    }

    connect(&mapper, SIGNAL(mapped(QString)), this, SLOT(open(QString)));

    n3SettingsExtrasController = new N3SettingsExtrasController();

    connect(stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(windowChanged(int)));
    LOG_DEBUG(l) << "connected windowChanged signal";

    engine.globalObject().setProperty("tweaks", engine.newQObject(this));

}

void TweaksPlugin::windowChanged(int index)
{
    LOG_TRACE(l);

    QWidget *currentWidget = const_cast<QWidget *>(stackedWidget->widget(index));
    if (currentWidget) {
        LOG_DEBUG(l) << "current widget: " << currentWidget->metaObject()->className() << " at " << currentWidget;

        HomePageView *homePageView = qobject_cast<HomePageView *>(currentWidget);
        if (homePageView) {
            LOG_DEBUG(l) << "found HomePageView, patching";

            if(pluginSettings->value("Tweaks/hideWishlist", false).toBool())
            {
                QWidget *footer = homePageView->findChild<QWidget *>("wishlistFooter");
                if (footer)
                    footer->hide();
            }

            if(pluginSettings->value("Tweaks/hideShop", false).toBool())
            {
                // hide header in top row
                QWidget *header = homePageView->findChild<QWidget *>("header");
                if(header)
                    header->hide();

                // hide seperator before sync
                QWidget *sep = homePageView->findChild<QWidget *>("lineLabel2");
                if (sep)
                    sep->hide();

                // hide sync entry
                FooterLabelWidget *sync = homePageView->findChild<FooterLabelWidget *>("sync");
                if (sync)
                    sync->hide();
            }

            FooterLabelWidget *readingLife = homePageView->findChild<FooterLabelWidget *>("readingLife");
            if (readingLife) {
                readingLife->setText(tr("Tools"));
                //disconnect(readingLife, 0, 0, 0);
                connect(readingLife, SIGNAL(clicked()), this, SLOT(patchReadingLife()), Qt::DirectConnection);
            }
        }

        //ReadingView *readingView = qobject_cast<ReadingView *>(currentWidget);
        //if (readingView) {
            //if(pluginSettings->value("Reader/tweakFooter", true).toBool()) {
                //connect(readv, SIGNAL(footerMenuOpened()), this, SLOT(bookFooterOpened()));
                //connect(readv, SIGNAL(footerMenuClosed()), this, SLOT(bookFooterClosed()));
            //}
        //}
    }
}

void TweaksPlugin::patchReadingLife()
{
    LOG_TRACE(l);
    FooterLabelWidget *readingLife = qApp->activeWindow()->findChild<FooterLabelWidget *>("readingLife");
    if(readingLife)
    {
        ReadingLifeMenuController* fbmc = readingLife->findChildren<ReadingLifeMenuController *>().last();
        NickelTouchMenu *ntm = readingLife->findChildren<NickelTouchMenu *>().last();

        if (fbmc && ntm)
        {
            // Clear menu and add entries based on configuration
            static QPoint origPos = ntm->pos();
            static int elemHeight = ntm->sizeHint().height()/2;

            ntm->clear();

            QList<MenuEntry>::iterator it;
            int activeEntries = 0;
            for(it = menuEntries.begin(); it != menuEntries.end(); it++) {
                if (it->enabled()) {
                    MenuTextItem *mti = NULL;
                    mti = fbmc->createMenuTextItem(ntm, it->name(), false, false);
                    mti->setSelected(false);
                    fbmc->addWidgetActionWithMapper(ntm, mti, &mapper, it->action(), true, true);
                    ntm->addSeparator();
                    activeEntries++;
                }
            }

            pluginSettings->beginGroup("CustomCommands");
            QStringList keys = pluginSettings->allKeys();

            QList<QString>::iterator keyit;
            for(keyit = keys.begin(); keyit != keys.end(); keyit++) {
                LOG_DEBUG(l) << "custom command: " << keyit->toStdString();
                QString action = pluginSettings->value(*keyit,QString()).toString();
                LOG_DEBUG(l) << "action: " << action.toStdString();

                MenuTextItem *mti = NULL;
                mti = fbmc->createMenuTextItem(ntm, *keyit, false, false);
                mti->setSelected(false);
                fbmc->addWidgetActionWithMapper(ntm, mti, &mapper, action, true, true);
                ntm->addSeparator();
                activeEntries++;
            }
            pluginSettings->endGroup();

            QPoint x = origPos;
            x.setY(origPos.y() + (2 - activeEntries) * elemHeight);
            x.setY(x.y() + (abs(2 - activeEntries) * 10));

            //cout << qPrintable(QString("x %1 | y %2").arg(x.x()).arg(x.y())) << endl << flush;

            ntm->move(x);
            ntm->update();
            ntm->layout();
        }
    }
}

void TweaksPlugin::open(QString request)
{
    LOG_TRACE(l);
    LOG_DEBUG(l) << "executing \"" << request.toStdString() << "\"";
    QString result = engine.evaluate(request).toString();
    LOG_DEBUG(l) << "result: " << result.toStdString();
}

void TweaksPlugin::hideShop(bool enable)
{
    LOG_TRACE(l) << enable;
    pluginSettings->setValue("Tweaks/hideShop", enable);
}

void TweaksPlugin::hideWishlist(bool enable)
{
    LOG_TRACE(l) << enable;
    pluginSettings->setValue("Tweaks/hideWishlist", enable);
}

bool TweaksPlugin::checkFirmwareVersion()
{
    QString requiredVersion = pluginSettings->value("Global/compatFirmware", REQUIRED_VERSION).toString();
    if (requiredVersion == "0.0.0") {
        LOG_DEBUG(l) << "disabling firmware version check";
        return true;	
    }

    // check if firmware version matches
    QFile f("/mnt/onboard/.kobo/version");
    if(!f.open(QIODevice::ReadOnly)) {
        LOG_ERROR(l) << "cannot read /mnt/onboard/.kobo/version";
        return false;
    }

    char szVersion[100];
    f.readLine(szVersion, 99);
    QString versionString(szVersion);
    QStringList versionParts = versionString.split(",");
    if(versionParts.size() != 6) {
        LOG_ERROR(l) << "error parsing /mnt/onboard/.kobo/version";
        return false;
    }

    // now check fw version
    if(versionParts[2] != requiredVersion) {
        LOG_ERROR(l) << "incompatible firmware version " << versionParts[2].toStdString() << ", required: " << requiredVersion.toStdString();
        return false;
    }

    return true;
}

QSharedPointer<QSettings> TweaksPlugin::settings() {
    return pluginSettings;
}

// Tool support functions

void TweaksPlugin::tweaksSettings() {
    TweaksSettingsView *tweaksSettingsView = new TweaksSettingsView(this, 0);
    TweaksSettingsPageView *tweaksSettingsPageView = new TweaksSettingsPageView(QApplication::activeWindow(), tweaksSettingsView, true);
    MainWindowController::sharedInstance()->pushView(tweaksSettingsPageView);
}

void TweaksPlugin::uninstallPlugin() {
    LOG_TRACE(l);
    QFile::remove("/usr/local/Trolltech/QtEmbedded-4.6.2-arm/plugins/imageformats/libtweaks.so");
    system("/bin/busybox reboot");
}

void TweaksPlugin::connectWifi() {
    WirelessWorkflowManager* p = WirelessWorkflowManager::sharedInstance();

    if(p) {
        if(p->isConnectedToHotspot())
            p->turnWifiOff();
        else
            p->connectWireless(false);
    }
}

void TweaksPlugin::toggleAirplaneMode() {
    WirelessWorkflowManager::sharedInstance()->setAirplaneMode(!WirelessWorkflowManager::sharedInstance()->isAirplaneMode());
}

void TweaksPlugin::executeSystemCmd(QString cmd)
{
    QProcess::execute(cmd);
}

void TweaksPlugin::sync(bool) {
    QStringList paths;
    paths << "/mnt/onboard/";
    N3FSSyncManager::sharedInstance()->sync(paths);
}

void TweaksPlugin::sleep() {
    N3PowerWorkflowManager::sharedInstance()->toggleSleep();
}

void TweaksPlugin::powerOff() {
   N3PowerWorkflowManager::sharedInstance()->powerOff(false);
}

void TweaksPlugin::shelves() {
    LibraryMenuController *pfv = QApplication::activeWindow()->findChildren<LibraryMenuController *>().last();
    if(pfv) {
        pfv->setShelvesView(true);
        pfv->shelves();
    }
}

void TweaksPlugin::readingLife() {
    ReadingLifeMenuController *pfv = QApplication::activeWindow()->findChildren<ReadingLifeMenuController *>().last();
    if(pfv) {
        pfv->setStatsView(true);
        pfv->stats();
    }
}

void TweaksPlugin::sudoku() {
    if(n3SettingsExtrasController)
        n3SettingsExtrasController->openSudoku();
}

void TweaksPlugin::chess() {
    if(n3SettingsExtrasController)
        n3SettingsExtrasController->openChess();
}

void TweaksPlugin::scribble() {
    if(n3SettingsExtrasController)
        n3SettingsExtrasController->openScribble();
}

void TweaksPlugin::openBrowser()
{
    WirelessWorkflowManager::sharedInstance()->openBrowser(QUrl());
}

Q_EXPORT_PLUGIN2(tweaks, TweaksPlugin)
