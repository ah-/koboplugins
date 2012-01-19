#ifndef __WIRELESS_MANAGER_H__
#define __WIRELESS_MANAGER_H__

class WirelessManager : public QObject {
public:
    static WirelessManager *sharedInstance();
};

#endif
