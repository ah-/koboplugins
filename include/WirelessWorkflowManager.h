#ifndef __WIRELESS_WORKFLOW_MANAGER_H__
#define __WIRELESS_WORKFLOW_MANAGER_H__

class WirelessWorkflowManager : public QObject {
public:
    static WirelessWorkflowManager *sharedInstance();
};

#endif
