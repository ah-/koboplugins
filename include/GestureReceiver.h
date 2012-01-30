#ifndef __GESTURE_RECEIVER_H__
#define __GESTURE_RECEIVER_H__

#include "GestureDelegate.h"

class GestureReceiver {
public:
    GestureReceiver();
    virtual ~GestureReceiver();

    virtual GestureDelegate *gestureDelegate();
    virtual void setGestureDelegate(GestureDelegate *);
    virtual void gestureEvent(QGestureEvent *);

private:
    GestureReceiver *gestureReceiver;
};

#endif
