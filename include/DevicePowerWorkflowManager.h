#ifndef __DEVICE_POWER_WORKFLOW_MANAGER_H__
#define __DEVICE_POWER_WORKFLOW_MANAGER_H__

class DevicePowerWorkflowManager : public QObject
{
	public:
		void powerOff(bool b);
		void toggleSleep();
};


#endif // __DEVICE_POWER_WORKFLOW_MANAGER_H__
