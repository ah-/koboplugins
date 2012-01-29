#ifndef __N3_SYNC_MANAGER_H__
#define __N3_SYNC_MANAGER_H__

class N3SyncManager : public QObject
{
	public:
	static N3SyncManager* sharedInstance();

	void sync();
};

#endif // ifndef __N3_SYNC_MANAGER_H__
