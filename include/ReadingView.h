#ifndef __READING_VIEW_H__
#define __READING_VIEW_H__

class ReadingView : public QWidget
    {
        Q_OBJECT

public:
    ReadingView(QWidget parent);
    ~ReadingView();

    void openFooterMenu();

signals:
    void footerMenuOpened();
};

#endif
