#ifndef __QT_SCRIPT_PLUGIN_INTERFACE_H__
#define __QT_SCRIPT_PLUGIN_INTERFACE_H__

#include <QObject>

class QtScriptPluginInterface
{
public:
	virtual ~QtScriptPluginInterface() {};

    // make an object accessible to the global qscript engine
	virtual void registerObject(QObject *object, QString name) = 0;
};

Q_DECLARE_INTERFACE(QtScriptPluginInterface, "com.foo.nickel.QtScriptPluginInterface/0.1")

#endif
