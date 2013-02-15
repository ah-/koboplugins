#ifndef __READING_VIEW_H__
#define __READING_VIEW_H__

class ReadingView : public QWidget
    {
        Q_OBJECT

public:
    ReadingView(QWidget parent);
    ~ReadingView();

    void showProgressBar();
    void hideProgressBar();

    void calculateChapterPositions();
    void processTap(QPoint);
    void nextChapter(bool);
    void nextPage();
    void prevPage();
   void openFooterMenu();

signals:
    void footerMenuOpened();
};

class ReadingMenuView : public QWidget
    {
        Q_OBJECT

public:
    ReadingMenuView(QWidget parent);
    ~ReadingMenuView();
};


#endif
