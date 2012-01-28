#ifndef __WIRELESS_WATCHDOG_H__
#define __WIRELESS_WATCHDOG_H__

class WirelessWatchdog : public QObject {
public:
    static WirelessWatchdog *sharedInstance();
	void setEnabled(bool enable);
};

#endif
