#ifndef __MAIN_WINDOW_CONTROLLER_H__
#define __MAIN_WINDOW_CONTROLLER_H__

class MainWindowController : public QObject {
public:
    static MainWindowController *sharedInstance();
    void popView(QWidget *);
};

#endif
