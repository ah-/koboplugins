#ifndef __FOOTER_LABEL_WIDGET_H__
#define __FOOTER_LABEL_WIDGET_H__

#include <QtGui>

class FooterLabelWidget : public QWidget
{
public:
    void setText(QString const&);

#ifdef KOBOFW_3_1_X
    void setPixmap(QString);
#endif

};

#endif

