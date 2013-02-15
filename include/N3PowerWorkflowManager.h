#ifndef __N3_POWER_WORKFLOW_MANAGER_H__
#define __N3_POWER_WORKFLOW_MANAGER_H__

#include "DevicePowerWorkflowManager.h"

class SleepViewControl;
class AbstractController;

class N3PowerWorkflowManager : public DevicePowerWorkflowManager
{
	public:
    static N3PowerWorkflowManager* sharedInstance();

    void plugged();
    void unplugged();
    void disconnectSleepView();
    void bookmarkSyncFailed();
    void bookmarkSyncSucceeded();
    void show(AbstractController*);
    void showBatteryLowView();
    void hideSleepView();
    bool sleepViewVisible() const;
    Volume getLatestVolume() const;
    void showSleepView();
    void aboutToShowSleep(SleepViewControl*);
    void showPowerOffView();

};

#endif // __N3_POWER_WORKFLOW_MANAGER_H__
