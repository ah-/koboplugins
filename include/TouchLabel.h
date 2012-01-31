#ifndef __TOUCH_LABEL_H__
#define __TOUCH_LABEL_H__

#include <QtGui>
#include "ReversibleWidget.h"

// size: 0x50
class TouchLabel : public QLabel, public ReversibleWidget<QLabel>, public GestureReceiver {
public:
    TouchLabel(const QString &, QWidget * = 0, QFlags<Qt::WindowType> = 0);
    TouchLabel(QWidget * = 0, QFlags<Qt::WindowType> = 0);
    virtual ~TouchLabel();

private:
    QPixmap p1;
    QPixmap p2;
    QPixmap p3;
    int unkown1;
    int unkown2;
    bool b1;
    bool b2;
};

//template<int s> struct size_check;
//size_check<sizeof(TouchLabel)> foo;

#endif

