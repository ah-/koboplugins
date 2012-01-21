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
#include "../qtscript/QtScriptPlugin.h"

class GestureDelegate {
};

class GestureReceiver {
public:
    void setGestureDelegate(GestureDelegate *);
    GestureDelegate *gestureDelegate();
};

class AbstractMenuController : public QObject, public GestureDelegate {
    Q_OBJECT
public:
    void addWidgetAction(QMenu *, QWidget *, char const *, bool, bool);
    void addWidgetActionWithMapper(QMenu *, QWidget *, QSignalMapper *, QString, bool, bool);
    void grabTapGesture(GestureReceiver *);
public slots:
    void show();
};


class MenuTextItem : public QWidget, public GestureReceiver {
public:
    void setSelectedImage(const QString &);
    void setSelected(bool);
    QString &text();
    void setText(const QString& t);
};

class AbstractNickelMenuController : public AbstractMenuController {
public:
    MenuTextItem * createMenuTextItem(QMenu *, const QString &, bool);
};

enum DecorationPosition { ONE };

class TouchMenu : public QMenu {
public:
    TouchMenu(QString const&, QWidget *);
    void initialize();
};

class NickelTouchMenu : public TouchMenu {
public:
    NickelTouchMenu(QWidget*, DecorationPosition);
};

class HomeMenuController : public AbstractNickelMenuController {
public:
    void store();
};

class TextHeader : public QWidget {
    Q_OBJECT
public:
    void setText(const QString &);
    QString &getText();
signals:
    void mouseDown();
    void mouseUp();
    void clicked();
};

class HomePageGridViewHeader : public QWidget {
public: 
    TextHeader *home();
};


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

    w = new TweaksWidget("Kobo Touch Tweaks", parent);
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
        MenuTextItem *mti = hmc->createMenuTextItem(ntm, QString("Tweaks"), false);
        mti->setSelectedImage(":/images/menu/trilogy_settings.png");
        mti->setSelected(true);
        hmc->addWidgetActionWithMapper(ntm, mti, &mapper, "application/x-kobo-tweaks", true, true);
        //hmc->addWidgetAction(ntm, mti, SLOT(store()), true, true);
        ntm->addSeparator();

        // store that we already patched this menu so the item doesn't get added twice
        lastPatchedMenu = (void *) ntm;
        cout << "TweaksPlugin::patchMenu(), success!" << endl << flush; 
    }
}

void TweaksPlugin::open(QString mimeType)
{
    cout << "TweaksPlugin::open(\"" << mimeType.toStdString() << "\")" << endl << flush; 
    Volume v;
    v.setMimeType(mimeType);
    v.setTitle("Tweaks");
    N3ReaderOpener *ro = (N3ReaderOpener*) ((ReadingViewMixin*) this)->createReader(v, NULL);
    ro->openReader();
}

Q_EXPORT_PLUGIN2(tictactoe, TweaksPlugin)
