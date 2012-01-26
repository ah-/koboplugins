#ifndef __CONFIG_H__
#define __CONFIG_H__


// Qt Includes
#include <QSettings>

// Windows Includes

// Linux Includes

// Library Includes

// Application Includes

// Defines

// Prototypes

/**************************************************************************************/

class PluginsConfig : public QSettings
    {
    public:
        static bool init(QString iniPath);
        static PluginsConfig* get();

    private:
        PluginsConfig(QString& fileName, Format format);

        static PluginsConfig* m_pInstance;
    };

/**************************************************************************************/

#endif // __CONFIG_H__
