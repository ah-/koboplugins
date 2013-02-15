#ifndef __QUILTED_VIEW_H__
#define __QUILTED_VIEW_H__

#include <QtGui>

class QuiltedView : public QWidget {
    Q_OBJECT

public:
	void setPixmap(QPixmap const&);
	void setBackgroundColor(QColor const&, QColor const&);
	void setBackgroundImage(QString const&, QColor const&);

};


#endif
