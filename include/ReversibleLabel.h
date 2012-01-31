#ifndef __REVERSIBLE_LABEL_H__
#define __REVERSIBLE_LABEL_H__

#include <QtGui>
#include "ReversibleWidget.h"

class ReversibleLabel : public QLabel, public ReversibleWidget<QLabel> {
public:
    ReversibleLabel(QWidget *);
    virtual ~ReversibleLabel();
};

template<int s> struct size_check;
size_check<sizeof(ReversibleLabel)> foo;

#endif
