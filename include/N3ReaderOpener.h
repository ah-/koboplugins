#ifndef __N3_READER_OPENER_H__
#define __N3_READER_OPENER_H__

#include <QtCore>
#include "Volume.h"

template <class T> class AbstractLibraryController;

class N3ReaderOpener : public QObject {
    Q_OBJECT
public:
    N3ReaderOpener(QObject *, Volume &volume, AbstractLibraryController<Volume> *);
    void openReader();
};

#endif
