#ifndef __TOUCH_LABEL_H__
#define __TOUCH_LABEL_H__

#include <QtGui>
#include "ReversibleWidget.h"
#include "GestureReceiver.h"

class TouchLabel : public QLabel, public ReversibleWidget<QLabel>, public GestureReceiver {
public:
    TouchLabel(const QString &, QWidget * = 0, QFlags<Qt::WindowType> = 0);
    TouchLabel(QWidget * = 0, QFlags<Qt::WindowType> = 0);
    virtual ~TouchLabel();

    void setSelectedPixmap(const QString&);
    void setDeselectedPixmap(const QString&);

signals:
    void tapped();

private:
    QPixmap p1;
    QPixmap p2;
    QPixmap p3;
    int unkown1;
    int unkown2;
    bool b1;
    bool b2;
};

#endif

