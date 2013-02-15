#ifndef __GRID_AND_LIBRARY_LIST_VIEW_H__
#define __GRID_AND_LIBRARY_LIST_VIEW_H__

#include "TextHeader.h"

class GridAndListLibraryView : public QObject {
    Q_OBJECT
};

class GridLibraryView : public QObject {
    Q_OBJECT
};

class LibraryViewHeader : public QWidget {
public: 
    TextHeader *menu();
};

#endif
