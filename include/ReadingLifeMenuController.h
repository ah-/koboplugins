#ifndef __READING_LIFE_MENU_CONTROLLER_H__
#define __READING_LIFE_MENU_CONTROLLER_H__

#include "HomeMenuController.h"

class ReadingLifeMenuController : public AbstractNickelMenuController {
public:
    void setStatsView(bool);
    void setAwardsView(bool);

    void awards();
    void stats();
};

#endif // __READING_LIFE_MENU_CONTROLLER_H__

