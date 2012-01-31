#ifndef __N3FSSYNC_MANAGER_H__
#define __N3FSSYNC_MANAGER_H__

class N3FSSyncManager : public QObject {
    Q_OBJECT
public:
    static N3FSSyncManager *sharedInstance();
public slots:
    void sync(const QStringList &);
};

#endif
