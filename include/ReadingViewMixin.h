#ifndef __READING_VIEW_MIXIN_H__
#define __READING_VIEW_MIXIN_H__

class Volume;
template <class T> class AbstractLibraryController;//<Volume>;

class ReadingViewMixin : public QObject {
    Q_OBJECT
public:
    void * /* N3ReaderOpener * */ createReader(Volume &volume, AbstractLibraryController<Volume> *);
    void showInformation(const Volume &);
private:
    ReadingViewMixin();
    ~ReadingViewMixin();
};

#endif
