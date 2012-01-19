#ifndef __SIGNAL_REGISTRY_H__
#define __SIGNAL_REGISTRY_H__

class SignalRegistry : public QObject {
public:
    static SignalRegistry *sharedInstance();
};

#endif
