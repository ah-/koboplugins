#ifndef __REVERSIBLE_WIDGET_H__
#define __REVERSIBLE_WIDGET_H__

#include <QtGui>

template <class T>
class ReversibleWidget {
public:
    virtual ~ReversibleWidget();
    
    void paintEvent(QPaintEvent *);
};

#endif
