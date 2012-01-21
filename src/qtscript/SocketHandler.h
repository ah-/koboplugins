#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QtCore>
#include <QtNetwork>
#include <QtScript>

#include "QtScriptPlugin.h"

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    SocketHandler(QTcpSocket *, QScriptEngine *engine, QtScriptPlugin *plugin);
signals:
    void open(QString);
private slots:
    void echo();
    void killMe();
private:
    QTcpSocket *clientSocket;
    QScriptEngine *engine;
};

#endif
