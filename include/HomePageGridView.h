#ifndef __HOME_PAGE_GRID_VIEW_H__
#define __HOME_PAGE_GRID_VIEW_H__

#include "TextHeader.h"

class HomePageGridView : public QObject {
    Q_OBJECT
};

class HomePageGridViewHeader : public QWidget {
public: 
    TextHeader *home();
};

class HomePageGridViewFooter : public QWidget {
};

#endif
