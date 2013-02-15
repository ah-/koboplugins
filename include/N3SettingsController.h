#ifndef __N3_SETTINGS_CONTROLLER_H__
#define __N3_SETTINGS_CONTROLLER_H__

class N3SettingsExtrasController : public QObject
{
public:
    void openBrowser();
    void openChess();
    void openSudoku();
    void openScribble();
};

class N3SettingsWirelessController : public QObject
{
public:
        void airplaneModeToggled();
};

#endif //__N3_SETTINGS_CONTROLLER_H__

