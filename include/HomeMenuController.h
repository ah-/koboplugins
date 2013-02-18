#ifndef __HOME_MENU_CONTROLLER_H__
#define __HOME_MENU_CONTROLLER_H__

#include "GestureDelegate.h"
#include "GestureReceiver.h"
#include "TouchLabel.h"

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
    MenuTextItem* createMenuTextItem(QMenu *, const QString &, bool, bool);
};

enum DecorationPosition { ONE };

class TouchMenu : public QMenu {
public:
    TouchMenu(QString const&, QWidget *);
    void initialize();
    void forceFullUpdate();
};

class NickelTouchMenu : public TouchMenu {
public:
    NickelTouchMenu(QWidget*, DecorationPosition);
    void setCustomPopupPosition(QPoint const&);
    void setCustomPopupPositionOffset(QPoint const&);
    void popupPosition(QWidget*);
};

class HomePageFooterController : public QObject
{
public:
    void library();
};


#endif // __HOME_MENU_CONTROLLER_H__

