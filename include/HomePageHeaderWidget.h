#ifndef __HOME_PAGE_HEADER_WIDGET_H__
#define __HOME_PAGE_HEADER_WIDGET_H__

#include "TextHeader.h"
#include "HomeMenuController.h"


class HomePageHeaderWidget : public QWidget {
public: 

    void setText(QString const& text);

};


class HomePageFooterView : public QWidget {
public: 
	void library();
    void libraryMouseDown();
};


class LibraryMenuController : public AbstractNickelMenuController
{
public: 
	void showLibrary();
    void shelves();
    void loadView();
    void setShelvesView(bool);
    void lists();
    void mine();

};


#endif
