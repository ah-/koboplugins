#ifndef __MENU_FACTORY_H__
#define __MENU_FACTORY_H__

class MenuFactory : public QObject {
public:
    static MenuFactory *sharedInstance();
};

#endif
