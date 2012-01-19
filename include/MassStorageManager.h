#ifndef __MASS_STORAGE_MANAGER_H__
#define __MASS_STORAGE_MANAGER_H__

class MassStorageManager : public QObject {
public:
    static MassStorageManager *sharedInstance();
};

#endif
