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

void hexdump(void *ptr, int buflen) {
  unsigned char *buf = (unsigned char*)ptr;
  int i, j;
  for (i=0; i<buflen; i+=16) {
      printf("%06x: ", i);
      for (j=0; j<16; j++) 
        if (i+j < buflen)
          printf("%02x ", buf[i+j]);
        else
          printf("   ");
      printf(" ");
      for (j=0; j<16; j++) 
        if (i+j < buflen)
          printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
      printf("\n");
    }
}

void QtScriptPlugin::open(QString mimetype)
{
    std::cout << thread() << endl << flush;
    //cout << "QtScriptPlugin::open(\"" << mimetype.toStdString() << "\")" << endl << flush; 
    //int *r8 = (int*) findChildByClassname(QApplication::activeWindow(), QString("N3SettingsExtrasController"));
    //cout << "QtScriptPlugin::open e:" << r8 << endl << flush; 
    //int *r0, *r1, *r3;

    Volume v;
    v.setMimeType("application/x-kobo-tweaks");
    v.setTitle("Sudoku");

    //asm("ldr %[result], [%[value], #0x10]" : [result] "=r" (r1) : [value] "r" (r8));
    //asm("add %[result], %[value], #0x10" : [result] "=r" (r0) : [value] "r" (r8));
    //asm("ldr %[result], [%[value]]" : [result] "=r" (r3) : [value] "r" (r1));
    //asm("mov %[result], %[value]" : [result] "=r" (r1) : [value] "r" (&v));
    //asm("mov r0, %[value]" : : [value] "r" (r0));
    //asm("mov r1, %[value]" : : [value] "r" (r1));
    //asm("mov r2, #0");
    //asm("mov r3, %[value]" : : [value] "r" (r3));
    // r3 = ReadingViewMixin::createReader(Volume &, AbstractLibraryController<Volume> *)
    //asm("blx %[value]" : : [value] "r" (r3));
    //cout << "alive" << endl << flush;
    //cout << "r1: " << r1 << endl << flush;
    //hexdump(r1, 100);
    //cout << "r0: " << r0 << endl << flush;
    //hexdump(r0, 100);
    //cout << "r3: " << r3 << endl << flush;
    //hexdump(r3, 100);
    void * foo = (void *) ((ReadingViewMixin*) this)->createReader(v, NULL);
    cout << "flush: " << foo << endl << flush;
    QObject *f = (QObject*)foo;
    cout << "flush: " << f << f->metaObject()->className() << endl << flush;
    N3ReaderOpener *ro = (N3ReaderOpener*) f;
    ro->openReader();
    //hexdump(foo, 100);
    //cout << "flush: " << foo << endl << flush;
    //cout << "r3: " << r3 << endl << flush;
    //asm("ldr %[result], [r0]" : [result] "=r" (r12));
    //asm("ldr %[result], [%[value], #0x30]" : [result] "=r" (r2) : [value] "r" (r12));
    //asm("blx %[value]" : : [value] "r" (r2));

    //QObject * q = (QObject *) r0;
    //cout << "QtScriptPlugin::open q:" << q << " " << q->metaObject()->className() << endl << flush; 
    

    //N3ReaderOpener r(homePageGridView, v, NULL);
    //r.openReader();
    
    //if (homePageGridView) {
        //ReadingViewMixin *rvm = qobject_cast<ReadingViewMixin *>(homePageGridView);
        //cout << "QtScriptPlugin::open rvm:" << rvm << endl << flush; 
        ////rvm->createReader(v, NULL);
        ////cout << "QtScriptPlugin::open createReader" << endl << flush; 
    //}

    //ReadingViewMixin::createReader(v, NULL);
    // TODO: pluginloader for mimetype, call reader on that?
    // readerfactory?
    // nach signals/slots die connected werden suchen
}

Q_EXPORT_PLUGIN2(tictactoe, QtScriptPlugin)
