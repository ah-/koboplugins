#ifndef __HOME_PAGE_GRID_VIEW_H__
#define __HOME_PAGE_GRID_VIEW_H__

class HomePageGridView : public QObject {
    Q_OBJECT
};


class TextHeader : public QWidget {
    Q_OBJECT
public:
    void setText(const QString &);
    QString &getText();
signals:
    void mouseDown();
    void mouseUp();
    void clicked();
};

class HomePageGridViewHeader : public QWidget {
public: 
    TextHeader *home();
};

class HomePageGridViewFooter : public QWidget {
};

#endif
