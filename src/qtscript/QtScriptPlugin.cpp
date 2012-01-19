#include "QtScriptPlugin.h"

#include <QtScript>
#include <iostream>

#include "SocketHandler.h"

using namespace std;

QtScriptPlugin::QtScriptPlugin()
{
    cout << "QtScriptPlugin()" << endl << flush; 
    // make the active window accessible by qtscript
    engine.globalObject().setProperty("aw", engine.newQObject(QApplication::activeWindow()));

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
