#ifndef __NAVIGATION_CONTROLLER_H__
#define __NAVIGATION_CONTROLLER_H__

class NavigationController : public QObject {
public:
    static NavigationController *sharedInstance();
};

#endif
