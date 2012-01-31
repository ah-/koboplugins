#ifndef __TOUCH_CHECK_BOX_H__
#define __TOUCH_CHECK_BOX_H__

#include <QtGui>
#include "GestureDelegate.h"
#include "GestureReceiver.h"

class TouchCheckBox : public QCheckBox, public GestureReceiver, public GestureDelegate {
public:
    TouchCheckBox(QWidget *);
};

#endif
