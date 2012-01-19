#ifndef __SPINNER_MANAGER_H__
#define __SPINNER_MANAGER_H__

class SpinnerManager : public QObject {
public:
    static SpinnerManager *sharedInstance();
};

#endif
