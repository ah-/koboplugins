#include "QtScriptPlugin.h"

#include <QtScript>
#include <iostream>

#include "SocketHandler.h"
#include "../../include/LedManager.h"
#include "../../include/MassStorageManager.h"
#include "../../include/MenuFactory.h"
#include "../../include/N3FSSyncManager.h"
#include "../../include/NavigationController.h"
#include "../../include/SignalRegistry.h"
#include "../../include/SpinnerManager.h"
#include "../../include/WirelessManager.h"
#include "../../include/WirelessWorkflowManager.h"

using namespace std;

QtScriptPlugin::QtScriptPlugin()
{
    cout << "QtScriptPlugin()" << endl << flush; 
    // make the active window accessible by qtscript
    engine.globalObject().setProperty("aw", engine.newQObject(QApplication::activeWindow()));
    engine.globalObject().setProperty("MassStorageManager", engine.newQObject(MassStorageManager::sharedInstance()));
    // deprecated?
    engine.globalObject().setProperty("MenuFactory", engine.newQObject(MenuFactory::sharedInstance()));
    engine.globalObject().setProperty("N3FSSyncManager", engine.newQObject(N3FSSyncManager::sharedInstance()));
    engine.globalObject().setProperty("NavigationController", engine.newQObject(NavigationController::sharedInstance()));
    engine.globalObject().setProperty("SignalRegistry", engine.newQObject(SignalRegistry::sharedInstance()));
    engine.globalObject().setProperty("SpinnerManager", engine.newQObject(SpinnerManager::sharedInstance()));
    engine.globalObject().setProperty("WirelessManager", engine.newQObject(WirelessManager::sharedInstance()));
    engine.globalObject().setProperty("WirelessWorkflowManager", engine.newQObject(WirelessWorkflowManager::sharedInstance()));

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
    new SocketHandler(clientConnection, &engine);
}

Q_EXPORT_PLUGIN2(tictactoe, QtScriptPlugin)
