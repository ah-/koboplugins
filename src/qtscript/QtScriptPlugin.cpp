#include "QtScriptPlugin.h"

#include <QtScript>
#include <iostream>


#include <ctype.h>
#include <stdio.h>

#include "SocketHandler.h"
#include "../../include/Content.h"
#include "../../include/Volume.h"
#include "../../include/ReadingViewMixin.h"
#include "../../include/N3ReaderOpener.h"

#include "../../include/LedManager.h"
#include "../../include/MassStorageManager.h"
#include "../../include/MenuFactory.h"
#include "../../include/N3FSSyncManager.h"
#include "../../include/NavigationController.h"
#include "../../include/SignalRegistry.h"
#include "../../include/SpinnerManager.h"
#include "../../include/WirelessManager.h"
#include "../../include/WirelessWatchdog.h"
#include "../../include/WirelessWorkflowManager.h"

using namespace std;

QtScriptPlugin::QtScriptPlugin()
{
    cout << "QtScriptPlugin()" << endl << flush; 
    // make the active window accessible by qtscript
    engine.globalObject().setProperty("qapp", engine.newQObject(QApplication::instance()));
    engine.globalObject().setProperty("aw", engine.newQObject(QApplication::activeWindow()));
    engine.globalObject().setProperty("MassStorageManager", engine.newQObject(MassStorageManager::sharedInstance()));
    // deprecated?
    engine.globalObject().setProperty("MenuFactory", engine.newQObject(MenuFactory::sharedInstance()));
    engine.globalObject().setProperty("N3FSSyncManager", engine.newQObject(N3FSSyncManager::sharedInstance()));
    engine.globalObject().setProperty("NavigationController", engine.newQObject(NavigationController::sharedInstance()));
    engine.globalObject().setProperty("SignalRegistry", engine.newQObject(SignalRegistry::sharedInstance()));
    engine.globalObject().setProperty("SpinnerManager", engine.newQObject(SpinnerManager::sharedInstance()));
    engine.globalObject().setProperty("WirelessManager", engine.newQObject(WirelessManager::sharedInstance()));
    engine.globalObject().setProperty("WirelessWatchdog", engine.newQObject(WirelessWatchdog::sharedInstance()));
    engine.globalObject().setProperty("WirelessWorkflowManager", engine.newQObject(WirelessWorkflowManager::sharedInstance()));

    engine.globalObject().setProperty("plugin", engine.newQObject(this));

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, 1337)) 
        cerr << "Unable to start server." << endl;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(getConnection()));
}

QtScriptPlugin::~QtScriptPlugin()
{
    cout << "~QtScriptPlugin()" << endl << flush; 
}

QStringList QtScriptPlugin::mimeTypes()
{
    cout << "QtScriptPlugin::mimeTypes()" << endl << flush; 

    // return something so the plugin gets loaded
	QStringList mimeTypes;
    mimeTypes << "application/x-qt-script";
	return mimeTypes;
}

QWidget *QtScriptPlugin::reader(void* plugin_state, QWidget *parent)
{
	return NULL;
}

ParserInterface *QtScriptPlugin::parser()
{
    return NULL;
}

void QtScriptPlugin::getConnection()
{
    clientConnection = tcpServer->nextPendingConnection();
    new SocketHandler(clientConnection, &engine, this);
}

void QtScriptPlugin::registerObject(QObject *object, QString name)
{
    engine.globalObject().setProperty(name, engine.newQObject(object));
}

QObject *findChildByClassname(QObject *o, QString name) {
    if (name == o->metaObject()->className()) {
        return o;
    }

    QListIterator<QObject*> it(o->children());
    while (it.hasNext()) {
        QObject *n = it.next();
        if (n) {
            QObject *r = findChildByClassname(n, name);
            if (r)
                return r;
        }
    }
    return 0;
}

void QtScriptPlugin::open(QString mimetype)
{
    std::cout << thread() << endl << flush;
    Volume v;
    v.setMimeType("application/x-kobo-tweaks");
    v.setTitle("Sudoku");
    void * foo = (void *) ((ReadingViewMixin*) this)->createReader(v, NULL);
    QObject *f = (QObject*)foo;
    N3ReaderOpener *ro = (N3ReaderOpener*) f;
    ro->openReader();
}

Q_EXPORT_PLUGIN2(tictactoe, QtScriptPlugin)
