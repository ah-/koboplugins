#ifndef __GESTURE_DELEGATE_H__
#define __GESTURE_DELEGATE_H__

#include <QtGui>
#include "DragGesture.h"
#include "TapGesture.h"
#include "SwipeGesture.h"

class GestureReceiver;

class GestureDelegate {
public:
    virtual void dragGesture(GestureReceiver *, DragGesture *);
    virtual void panGesture(GestureReceiver *, QPanGesture *);
    virtual void pinchGesture(GestureReceiver *, QPinchGesture *);
    virtual void swipeGesture(GestureReceiver *, SwipeGesture *);
    virtual void tapGesture(GestureReceiver *, TapGesture *);
};

#endif
