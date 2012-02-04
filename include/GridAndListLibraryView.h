#ifndef __GRID_AND_LIBRARY_LIST_VIEW_H__
#define __GRID_AND_LIBRARY_LIST_VIEW_H__

class GridAndListLibraryView : public QObject {
    Q_OBJECT
};


class LibraryViewHeader : public QWidget {
public: 
    TextHeader *menu();
};



#endif
