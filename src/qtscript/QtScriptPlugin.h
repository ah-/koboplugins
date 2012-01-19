#ifndef __QT_SCRIPT_PLUGIN_H__
#define __QT_SCRIPT_PLUGIN_H__

#include <QtGui>
#include <QtNetwork>
#include <QtScript>
#include "../../include/PluginInterface.h"

class QtScriptPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

public:
	QtScriptPlugin();
	~QtScriptPlugin();

	QStringList mimeTypes();
	QWidget *reader(void* plugin_state, QWidget *parent = 0);
	ParserInterface *parser();

private slots:
    void getConnection();

private:
    QScriptEngine engine;
    QTcpSocket *clientConnection;
    QTcpServer *tcpServer;
};

#endif
