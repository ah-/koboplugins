#ifndef __DEVICE_POWER_WORKFLOW_MANAGER_H__
#define __DEVICE_POWER_WORKFLOW_MANAGER_H__

class DevicePowerWorkflowManager : public QObject
{
	public:
		void powerOff(bool b);
		void toggleSleep();
};


class N3PowerWorkflowManager : public QObject
{
	public:
		static N3PowerWorkflowManager* sharedInstance();

		void showPowerOffView();
		void showSleepView();
};

#endif // __DEVICE_POWER_WORKFLOW_MANAGER_H__
