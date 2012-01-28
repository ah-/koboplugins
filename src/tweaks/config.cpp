/**************************************************************************************
 *
 * File < config.cpp > created < Jan 26, 2012 > by < mast >
 *
 **************************************************************************************/

// Qt Includes

// Std Includes
#include <iostream>

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes
#include "config.h"

// Defines

// Globals

/**************************************************************************************/
using namespace std;


PluginsConfig* PluginsConfig::m_pInstance = NULL;

bool PluginsConfig::init(QString iniPath)
    {
    if(!m_pInstance)
        m_pInstance = new PluginsConfig(iniPath, QSettings::IniFormat);

    return true;
    }

PluginsConfig* PluginsConfig::get()
    {
    if(!m_pInstance)
        {
        cout << "Call ::init first" << endl << flush;
        return NULL;
        }

    return m_pInstance;
    }

PluginsConfig::PluginsConfig(QString& fileName, Format format)
    : QSettings(fileName, format)
    {
    }

/**************************************************************************************/
