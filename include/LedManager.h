#ifndef __LED_MANAGER_H__
#define __LED_MANAGER_H__

class LedManager {
public:
    static LedManager *sharedInstance();

    void blueOff() const;
    void blueOn() const;
    void greenOff() const;
    void greenOn() const;
    void off() const;
    void redOff() const;
    void redOn() const;
};

#endif
