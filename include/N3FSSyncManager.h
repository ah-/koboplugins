#ifndef __N3FSSYNC_MANAGER_H__
#define __N3FSSYNC_MANAGER_H__

class N3FSSyncManager : public QObject {
public:
    static N3FSSyncManager *sharedInstance();
};

#endif
