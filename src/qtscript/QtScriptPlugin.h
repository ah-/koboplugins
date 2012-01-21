#ifndef __QT_SCRIPT_PLUGIN_H__
#define __QT_SCRIPT_PLUGIN_H__

#include <QtGui>
#include <QtNetwork>
#include <QtScript>
#include "../../include/PluginInterface.h"
#include "../../include/QtScriptPluginInterface.h"

class QtScriptPlugin : public QObject, public PluginInterface, public QtScriptPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface QtScriptPluginInterface)

public:
	QtScriptPlugin();
	~QtScriptPlugin();

	QStringList mimeTypes();
	QWidget *reader(void* plugin_state, QWidget *parent = 0);
    ParserInterface *parser();

    void registerObject(QObject *object, QString name);

public slots:
    void open(QString mimetype);

private slots:
    void getConnection();

private:
    QScriptEngine engine;
    QTcpSocket *clientConnection;
    QTcpServer *tcpServer;
};

#endif
