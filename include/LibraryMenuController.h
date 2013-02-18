#ifndef __LIBRARY_MENU_CONTROLLER_H__
#define __LIBRARY_MENU_CONTROLLER_H__

#include "HomeMenuController.h"

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
