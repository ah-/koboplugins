#ifndef __WIRELESS_WORKFLOW_MANAGER_H__
#define __WIRELESS_WORKFLOW_MANAGER_H__

#include <QtCore>

class WirelessWorkflowManager : public QObject {
    Q_OBJECT
public:
    static WirelessWorkflowManager *sharedInstance();

signals:
    void connectingFailed();
    void onNetworkConnected();

public slots:
    void openBrowser(const QUrl&);
    void connectWireless(bool bConnect);
    void connectWirelessSilently();
    void turnWifiOff();
    void turnWifiOn();
    void setAirplaneMode(bool bAirMode);

    bool isAirplaneMode();
    bool isConnectedToHotspot();
};

#endif
