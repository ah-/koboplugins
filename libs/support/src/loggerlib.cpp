/* loggerlib.cpp - ....

History
* -------------------------
*
*  $Log: lib.cpp  $

*/

/*
	Todo list

*/


// QT Includes
#include <QTime>
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDir>
#include <QString>
#include <QRegExp>
#include <QFileInfoList>
#include <QProcess>
#include <QThread>

// Includes
#include "msgq_i.h"
#include "loggerlib.h"
#include "loggerlib_i.h"


// Defines


// Local Variables


// Prototypes



/**************************************************************************************/

LoggerVersion::LoggerVersion(const char* szLib, const char* szFile, const char* szDate, const char* szTime, const char* szRevision, const char *LoggerConfig)
	{
	Logger::logPureCfg((LoggerConfig ? LoggerConfig : DEFAULT_LOGGER_CONFIG), "*** LIBRARY <%s>: FILE <%s> --> REVISION <%s> --> COMPILE TIME <%s> <%s> ***",
					szLib, szFile, szRevision, szDate, szTime);
	}

LoggerVersion::~LoggerVersion()
	{
	}

/**************************************************************************************/

/*!
	\class ILogObject
	\brief Interface for logging objects

	Specifies the interface for new logging objects
	\sa FileLogger
	\sa ConsoleLogger

 */

ILogObject::ILogObject()
	{}


ILogObject::~ILogObject()
	{}


/* --------------------------------------------------------------------*/


/*!
    \class ConsoleLogger
    \brief Log object for logging to 'stdout'

    Predefined log object which can be used to send log messages to 'stdout'.

    \sa ILogObject
 */


ConsoleLogger::ConsoleLogger() : m_qout( stdout )
    {}

ConsoleLogger::~ConsoleLogger()
    {}

void ConsoleLogger::outputText( QString strText )
    {
    m_qout << strText << endl;
    }


/* --------------------------------------------------------------------*/


/*!
	\class FileLogger
	\brief Log object for file logging

	Predefined log object which can be used to send log messages to a text file.
	The path is freely configurable.

	\sa ILogObject
 */


FileLogger::FileLogger( QString strPath, bool bForceClean)
	{
    QMutexLocker lock(&m_hMutex);
    
    QDir dir( strPath );
    if (!dir.exists())
        {
        if (!dir.mkpath(dir.absolutePath()))
            {
            // fixme Error handling fehlt, was kann ich hier machen?
            return;
            }
        }

    m_strLogRootDir = dir.absolutePath();

    setLogFile(bForceClean);
    }


FileLogger::~FileLogger()
	{
    m_logFile.close(); // notwendig?
    }


void FileLogger::outputText( QString strText )
	{
    QMutexLocker lock(&m_hMutex);
    QDate actDate = QDate::currentDate();
    const int dMaxFileSize = 5048676; // 5 MB in Byte

    if (actDate != m_logFileDate)
        {
        setLogFile();
        }

    if (m_logFile.size() > dMaxFileSize)
        {
        // printf("--> Logfile zu groß\n");
        // Zuerst altes bak-File löschen (falls vorhanden)
        QString strFileBak(m_strLogRootDir);
        strFileBak.append("/log-" + m_logFileDate.toString("dd-MM-yyyy") + ".bak");
        QFile fileBak(strFileBak);
        if (fileBak.exists())
            {
            // printf("remove bak-file for %s\n", qPrintable(m_logFile.fileName()));
            fileBak.remove();
            }
        else
            {
            // printf("bak-file %s for %s does not exist\n",
            //        qPrintable(fileBak.fileName()), qPrintable(m_logFile.fileName()));
            }

        // Dann bisheriges Log-File auf bak umbenennen
        m_logFile.rename(strFileBak);

        // "neues" Logfile aufmachen
        setLogFile();
        }

    QTextStream logOut(&m_logFile);
    logOut << strText << endl; // newline and flush
	}


// FileLogger private Funktionen

void FileLogger::setLogFile(bool bForceClean)
    {
    m_logFile.close();
    m_logFileDate = QDate::currentDate();

    QString strFileName(m_strLogRootDir);
    strFileName.append("/log-" + m_logFileDate.toString("dd-MM-yyyy") + ".txt");

    m_logFile.setFileName(strFileName);
    if (!m_logFile.open(QIODevice::WriteOnly | ((bForceClean) ? QIODevice::Truncate : QIODevice::Append) | QIODevice::Text))
        {
        // fixme Error handling fehlt, was kann ich hier machen?
        return;
        }
    cleanupLogFiles(30);
    }


void FileLogger::cleanupLogFiles(int nKeepDays)
    {
    QDir dir(m_strLogRootDir, "*.*");
    dir.setFilter(QDir::Files);

    // QDate Format dd-MM-yyyy
    QRegExp rx("^log-(\\d{2})-(\\d{2})-(\\d{4})\\.(txt|bak)$");
    QStringList list = dir.entryList();
    for (int i = 0; i < list.size(); i++)
        {
        QString fileName = list.at(i);

        if (rx.indexIn(fileName) >= 0)
            {
            // printf("<%s> --- <%s>|<%s>|<%s>\n", qPrintable(fileName),
            //        qPrintable(rx.cap(1)), qPrintable(rx.cap(2)), qPrintable(rx.cap(3)));
            int nDay = rx.cap(1).toInt();
            int nMonth = rx.cap(2).toInt();
            int nYear = rx.cap(3).toInt();
            QDate fileDate(nYear, nMonth, nDay);
            if (fileDate.daysTo(m_logFileDate) > nKeepDays)
                {
                QFile tmpFile(m_strLogRootDir + "/" + fileName);
                // printf("--> delete %s\n", qPrintable(tmpFile.fileName()));
                tmpFile.remove();
                }
            }
        else
            {
            // printf("<%s> --- no match, do nothing\n", qPrintable(fileName));
            }
        }
    }



/* --------------------------------------------------------------------*/

/*!
	\class Logger
	\brief Wrapper class for LoggerBase

	Logging object used in the application. The logging slot must be defined in the constructor.\n
	Supports logging based on logging level. The log function support variable argument lists with the same syntax as 'printf' \n
	The text formatting is done within the log routine.

 */


Logger::Logger( QString strSlot ) : m_strSlot( strSlot )
    {
    }

Logger::Logger( QString strSlot, QString strFuncName ) : m_strSlot( strSlot )
	{
	m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );
	}

Logger::~Logger()
	{}

void Logger::logPure( QString strText, ... )
	{
	if(!LoggerBase::get()->enabled())
		return;

	va_list args;
	va_start( args, strText );

	format( strText, args );

	LoggerBase::get()->log( QString(DEFAULT_LOGGER_CONFIG), strText );
	}

void Logger::logPureCfg( QString LoggerConfig, QString strText, ... )
    {
    if(!LoggerBase::get()->enabled())
        return;

    va_list args;
    va_start( args, strText );

    format( strText, args );

    LoggerBase::get()->log( (LoggerConfig.size() ? LoggerConfig : QString(DEFAULT_LOGGER_CONFIG)), strText );
    }
void Logger::logLevel( QString strSlot, int nLevel, QString strText, ... )
    {
    if(!LoggerBase::get()->enabled())
        return;

    if(nLevel > LoggerBase::get()->getLogLevel( strSlot ))
        return;

    va_list args;
    va_start( args, strText );

    format( strText, args );

//  LoggerBase::get()->log( strSlot, nLevel, strText );
    LoggerBase::get()->log( QString(DEFAULT_LOGGER_CONFIG), strText ); // haben Level schon geprüft
    }

void Logger::logLevelCfg( QString LoggerConfig, QString strSlot, int nLevel, QString strText, ... )
	{
	if(!LoggerBase::get()->enabled())
		return;

	if(nLevel > LoggerBase::get()->getLogLevel( strSlot ))
		return;

	va_list args;
	va_start( args, strText );

	format( strText, args );

//	LoggerBase::get()->log( strSlot, nLevel, strText );
	LoggerBase::get()->log( (LoggerConfig.size() ? LoggerConfig : QString(DEFAULT_LOGGER_CONFIG)) , strText ); // haben Level schon geprüft
	}

void Logger::log( int nLevel, QString strText, ... )
	{
	if(!LoggerBase::get()->enabled())
		return;

	if(nLevel > LoggerBase::get()->getLogLevel( m_strSlot ))
		return;

	va_list args;
	va_start( args, strText );

	format( strText, args );

// 	LoggerBase::get()->log( m_strSlot, nLevel, strText );
	LoggerBase::get()->log( QString(DEFAULT_LOGGER_CONFIG), strText ); // haben Level schon geprüft
	}

void Logger::logCfg( QString LoggerConfig, int nLevel, QString strText, ... )
    {
    if(!LoggerBase::get()->enabled())
        return;

    if(nLevel > LoggerBase::get()->getLogLevel( m_strSlot ))
        return;

    va_list args;
    va_start( args, strText );

    format( strText, args );

//  LoggerBase::get()->log( m_strSlot, nLevel, strText );
    LoggerBase::get()->log( (LoggerConfig.size() ? LoggerConfig : QString(DEFAULT_LOGGER_CONFIG)), strText ); // haben Level schon geprüft
    }

int Logger::getLogLevel()
	{
	return LoggerBase::get()->getLogLevel( m_strSlot );
	}

void Logger::format( QString& strText, va_list args )
	{
    // Non-Qt-Applikationen krachen im Logger, weil currentThread() NULL liefert
    const QThread *pThread = QThread::currentThread();

    QString threadName;
    if (pThread)
        {
        threadName = pThread->objectName();
        if (threadName.size() <= 0)
            threadName.sprintf("%p", pThread);
        }
    else
        threadName = "Non-Qt";

	strText = threadName + ": "
        + LoggerBase::get()->currentTime() + ": "
        + strText.vsprintf( strText.toAscii().data(), args );

	va_end( args );
	}


/*!
	Simplifies the remembered __PRETTY_FUNCTION__ zString to the format: 'ClassName::FunctionName'
	e.g.
	Input:  "static bool DryAdjusts::G0Z0DryAdjust::defineAdjustPointPosition(unsigned int, GVec1D &)"
	Output: "DryAdjusts::G0Z0DryAdjust::defineAdjustPointPosition"
 */
QString Logger::getSimplifiedFunctionName( QString& funcName )
	{
    m_strSimplifiedFuncName	= funcName;

    // Beispiele:
    // QList<void (*)(PL_CHANGE_TYPE, QStringList)> ParamLib::addChangeHook(void (*)(PL_CHANGE_TYPE, QStringList))
    // static QList<QPair<double, bool> > parseLinePacketSpec(QString str)

//     QRegExp rx(" ([^( ]+)\\(");
    QRegExp rx("(^| )([^( ]+)\\(");

    int pos = rx.indexIn(funcName);
    if (pos > -1)
        m_strSimplifiedFuncName = rx.cap(2);

	return m_strSimplifiedFuncName;


//     m_strSimplifiedFuncName	= funcName;
//     m_strSimplifiedFuncName.remove(QRegExp("^ ?((static|virtual) +)?[^ :]+ "));
// 	m_strSimplifiedFuncName = m_strSimplifiedFuncName.mid( 0, m_strSimplifiedFuncName.indexOf( '(' ) );

//     if (0 == m_strSimplifiedFuncName.size() && funcName.size() > 0)
//         {
//         // Irgendwas ging schief, bspw:

//         QRegExp rx("[^ ()<>]+::[^ ()<>]+");

//         int pos = rx.indexIn(funcName);
//         if (pos > -1)
//             m_strSimplifiedFuncName = rx.cap(0);
//         else
//             m_strSimplifiedFuncName = funcName;
//         }

// 	return m_strSimplifiedFuncName;
	}


/*!
	Helper for HERE-Macro.
	Output: 'ExampleClass::ExampleMethod (732) -> '
 */
QString Logger::getExactOrigin( unsigned int unLine )
	{
	return m_strSimplifiedFuncName + " (" + QString().setNum( unLine ) + ") -> ";
	}


/*!
	Helper for LOG_WARNING-Macro.
	Output: 'ExampleClass::ExampleMethod (732) -> Warning: ExampleText'
 */
QString Logger::getWarningText( QString strFuncName, unsigned int unLine)
	{
    if (0 == m_strSimplifiedFuncName.size())
        m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );

	return getExactOrigin( unLine ) + "Warning:: ";
	}

QString Logger::getInfoText(QString strFuncName, unsigned int unLine)
	{
    if (0 == m_strSimplifiedFuncName.size())
        m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );

	return getExactOrigin( unLine ) + "Info:: ";
	}

QString Logger::getClassText(QString strFuncName, unsigned int unLine)
    {
    if (0 == m_strSimplifiedFuncName.size())
        m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );

	return m_strSimplifiedFuncName + " (" + QString().setNum( unLine ) + "): ";
    }


/*!
	Helper for GOTO_ERROR-Macro.
	Output: 'ExampleClass::ExampleMethod (732) -> Error occured!'
 */
QString Logger::getErrorText( QString strFuncName, unsigned int unLine )
	{
    if (0 == m_strSimplifiedFuncName.size())
        m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );

	return getExactOrigin( unLine ) + "Error occured!";
	}


/*!
	Helper for GOTO_AND_LOG_ERROR- and LOG_ERROR- Macros.
	Output: 'ExampleClass::ExampleMethod (732) -> Error: ExampleText'
 */
QString Logger::getErrorTextCustom(QString strFuncName, unsigned int unLine)
	{
    if (0 == m_strSimplifiedFuncName.size())
        m_strSimplifiedFuncName = getSimplifiedFunctionName( strFuncName );

	return getExactOrigin( unLine ) + "Error:: ";
	}

/* --------------------------------------------------------------------*/


LoggerSlot::LoggerSlot( QString strSlot, int nStartLogLevel ) : m_strSlot(strSlot)
    {
    LoggerBase::get()->addSlot( m_strSlot, nStartLogLevel);
    }


LoggerSlot::~LoggerSlot(  )
    {
    LoggerBase::get()->removeSlot(m_strSlot);
    }

/* --------------------------------------------------------------------*/


// initialize static member variables
/*
QMap<QString, int> LoggerBase::m_mapSlots;
LoggerThread LoggerBase::m_LoggerThread;
RMsgQ<QString> LoggerBase::m_qLog( 10 );
QMutex LoggerBase::m_SlotMutex;
ConsoleLogger LoggerBase::console;
*/
LoggerBase* LoggerBase::m_pBase = 0;

LoggerBase* LoggerBase::get()
	{
	if(!m_pBase)
		m_pBase = new LoggerBase();

	return m_pBase;
	}

LoggerBase::LoggerBase()
	{
	m_pLogQ = new RMsgQIntern<RLoggerData>;
	m_pLoggerThread = new LoggerThread;
	m_bLogging = false;

    reinitTimer();
	}

LoggerBase::~LoggerBase()
	{}

ILogObject* LoggerBase::getLogObject( QString ObjectName )
    {
    return m_pLoggerThread->getLogObject(ObjectName);
    }

void LoggerBase::addLogObject( QString strObjName, ILogObject* pLogObj, QString LoggerConfig )
	{
	// pass object through to logger thread
	m_pLoggerThread->addLogObject( strObjName, pLogObj, LoggerConfig );
	}

void LoggerBase::removeLoggerConfig( QString LoggerConfig )
    {
    m_pLoggerThread->removeLoggerConfig( LoggerConfig );
    }

void LoggerBase::removeLogObject( QString strObjName, QString LoggerConfig )
	{
	// remove object from logger thread
	m_pLoggerThread->removeLogObject( strObjName, LoggerConfig );
	}

void LoggerBase::addSlot( QString strSlot, int nDefaultLevel )
	{
	QMutexLocker lock(&m_SlotMutex);

	if ( !m_mapSlots.contains( strSlot ) )
		m_mapSlots.insert( strSlot, nDefaultLevel );
	}

void LoggerBase::removeSlot( QString strSlot )
	{
	QMutexLocker lock(&m_SlotMutex);

	if ( m_mapSlots.contains( strSlot ) )
		m_mapSlots.remove( strSlot );
	}

void LoggerBase::start()
	{
	if(!m_bLogging)
		{
		m_bLogging = true;
		m_pLoggerThread->setMsgQ( m_pLogQ );
		m_pLoggerThread->start();
		}
	}

void LoggerBase::stop()
	{
	m_bLogging = false;

	log(QString(DEFAULT_LOGGER_CONFIG), QString(LOGGER_KILL_COMMAND));
	m_pLoggerThread->wait();
	}

bool LoggerBase::configEnabled( QString LoggerConfig )
    {
    return m_pLoggerThread->configEnabled(LoggerConfig);
    }

bool LoggerBase::enabled()
	{
	return m_bLogging;
	}

bool LoggerBase::withThreadname( QString LoggerConfig )
    {
    return m_pLoggerThread->withThreadname( LoggerConfig );
    }

bool LoggerBase::withTimestamp( QString LoggerConfig )
    {
    return m_pLoggerThread->withTimestamp( LoggerConfig );
    }

void LoggerBase::setLogLevel( QString strSlot, int nLevel )
	{
	QMutexLocker lock(&m_SlotMutex);

	if ( m_mapSlots.contains( strSlot ) )
		m_mapSlots.insert( strSlot, nLevel );
	}

int LoggerBase::getLogLevel ( QString strSlot )
	{
	QMutexLocker lock(&m_SlotMutex);

	QMap<QString, int>::iterator it = m_mapSlots.find( strSlot );

	if ( it != m_mapSlots.end() )
		return it.value();

	return 0;
	}

void LoggerBase::log( QString LoggerConfig, QString strSlot, int nLevel, QString strText )
	{
	QMutexLocker lock(&m_SlotMutex);

	QMap<QString, int>::iterator it = m_mapSlots.find( strSlot );

	if ( it != m_mapSlots.end() && it.value() >= nLevel )
		log( LoggerConfig, strText );
	}

void LoggerBase::log( QString LoggerConfig, QString strText )
	{
    RLoggerData Data;
    Data.setData( QString(strText.toLatin1()) );
	Data.setLoggerConfig( LoggerConfig );
	m_pLogQ->send( Data );
	}

void LoggerBase::listSlots()
	{
	QMutexLocker lock(&m_SlotMutex);

	QMap<QString, int>::iterator it;
	QString str;

	logIntern( "" );
	logIntern( "***** BEGIN REGISTERED LOGGING SLOTS *****" );
	logIntern( "" );

	for ( it = m_mapSlots.begin(); it != m_mapSlots.end(); it++ )
		{
			logIntern( str.sprintf( "LoggingSlot < %s , %d >" ,
		                    it.key().toAscii().data(), it.value() ) );
		}

	logIntern( "" );
	logIntern( "***** END REGISTERED LOGGING SLOTS *****" );
	logIntern( "" );
	}

QMap<QString, int> LoggerBase::getSlots()
	{
	return m_mapSlots;
	}

void LoggerBase::logIntern(QString str)
	{
	console.outputText(str);
	}


QString LoggerBase::currentTime()
    {
    int ela = int(m_perfTimer.elapsed());
    if (ela > 3600000 || ela < 0) // einmal pro Stunde
        {
        reinitTimer();
        ela = int(m_perfTimer.elapsed());
        }
    return m_initialTime.addMSecs(ela).toString("hh:mm:ss:zzz");
    }


void LoggerBase::reinitTimer()
    {
    m_initialTime = QTime::currentTime();
    m_perfTimer.elapsed(true);
    }

void LoggerBase::setConfigEnabled ( QString LoggerConfig, bool Value )
    {
    m_pLoggerThread->setConfigEnabled(LoggerConfig, Value);
    }

void LoggerBase::setWithThreadname( QString LoggerConfig, bool Value )
    {
    m_pLoggerThread->setWithThreadname( LoggerConfig, Value );
    }

void LoggerBase::setWithTimestamp( QString LoggerConfig, bool Value )
    {
    m_pLoggerThread->setWithTimestamp( LoggerConfig, Value);
    }
/* --------------------------------------------------------------------*/


RLoggerData::RLoggerData() : m_Data(QString(DEFAULT_LOGGER_DATA)), m_LoggerConfig(QString(DEFAULT_LOGGER_CONFIG))
    {
    }

RLoggerData::RLoggerData(QString Data, QString LoggerConfig) : m_Data(Data), m_LoggerConfig(LoggerConfig)
    {    
    }

RLoggerData::~RLoggerData()
    {
    }

QString RLoggerData::data()
    {
    return m_Data;
    }

QString RLoggerData::loggerConfig()
    {
    return m_LoggerConfig;
    }

void RLoggerData::setData(QString Value)
    {
    m_Data = Value;
    }

void RLoggerData::setLoggerConfig(QString Value)
    {
    m_LoggerConfig = Value;
    }

/* --------------------------------------------------------------------*/

/*!
    \class RLoggerConfig
    \brief Encapsulates a Logger- Configuration.

    Used to define a set of Loggers

 */

RLoggerConfig::RLoggerConfig() : m_Enabled(true), m_WithThreadname(true), m_WithTimestamp(true)
    {
    }

RLoggerConfig::RLoggerConfig( QString Name ) : m_Enabled(true),
    m_WithThreadname(true), m_WithTimestamp(true), m_Name(Name)
    {
    }

RLoggerConfig::~RLoggerConfig()
    {
    }

bool RLoggerConfig::enabled()
    {
    return m_Enabled;
    }

bool RLoggerConfig::withThreadname()
    {
    return m_WithThreadname;
    }

bool RLoggerConfig::withTimestamp()
    {
    return m_WithTimestamp;
    }

QString RLoggerConfig::name()
    {
    return m_Name;
    }

const QStringList& RLoggerConfig::loggers()
    {
    return m_Loggers;
    }

void RLoggerConfig::setEnabled( bool Value )
    {
    m_Enabled = Value;
    }

void RLoggerConfig::setWithThreadname( bool Value )
    {
    m_WithThreadname = Value;
    }
    
void RLoggerConfig::setWithTimestamp( bool Value )
    {
    m_WithTimestamp = Value;
    }

void RLoggerConfig::addLogger( QString Name )
    {
    if ( !m_Loggers.contains(Name) )
        m_Loggers.append(Name);
    }

void RLoggerConfig::removeLogger( QString Name )
    {
    if ( m_Loggers.contains(Name) )
        m_Loggers.removeAll(Name);
    }


/* --------------------------------------------------------------------*/

/*!
    \class LoggerThread

 */


LoggerThread::LoggerThread()
	{
	m_mapLogObj.clear();
	m_LoggerConfigs.clear();
	}

bool LoggerThread::configEnabled( QString LoggerConfig )
    {
    return (m_LoggerConfigs.contains(LoggerConfig) ? m_LoggerConfigs[LoggerConfig].enabled() : false);
    }

bool LoggerThread::withThreadname( QString LoggerConfig )
    {
    return (m_LoggerConfigs.contains(LoggerConfig) ? m_LoggerConfigs[LoggerConfig].withThreadname() : false);
    }

bool LoggerThread::withTimestamp( QString LoggerConfig )
    {
    return (m_LoggerConfigs.contains(LoggerConfig) ? m_LoggerConfigs[LoggerConfig].withTimestamp() : false);
    }

ILogObject* LoggerThread::getLogObject( QString ObjectName )
    {
    QMutexLocker lock(&m_ObjMutex);
    return m_mapLogObj[ObjectName]; 
    }

void LoggerThread::setMsgQ( RMsgQIntern<RLoggerData>* qLog )
	{
	m_pLogQ = qLog;
	}


void LoggerThread::addLogObject( QString strObjName, ILogObject* pLogObj, QString LoggerConfig )
	{
	QMutexLocker lock(&m_ObjMutex);
	
	if ( !pLogObj || !LoggerConfig.size() )
	    return;

	if ( !m_mapLogObj.contains( strObjName ) )
		m_mapLogObj.insert( strObjName, pLogObj );
	
	if ( !m_LoggerConfigs.contains( LoggerConfig ) )
	    m_LoggerConfigs.insert(LoggerConfig, RLoggerConfig( LoggerConfig ) );

	m_LoggerConfigs[LoggerConfig].addLogger( strObjName );
	}

void LoggerThread::removeLoggerConfig(QString LoggerConfig)
    {
    QMutexLocker lock(&m_ObjMutex);
    
    if ( LoggerConfig == QString(DEFAULT_LOGGER_CONFIG) || !m_LoggerConfigs.contains( LoggerConfig ) ) return;
    
    const QStringList LoggerNames = m_LoggerConfigs[LoggerConfig].loggers();
    
    for ( QStringList::const_iterator itL = LoggerNames.begin(); itL != LoggerNames.begin(); itL++ )
        {
        bool Remove = true;
        QString ObjectName = *itL;
        
        for ( QMap<QString, RLoggerConfig>::iterator itC = m_LoggerConfigs.begin(); itC != m_LoggerConfigs.end(); itC++ )
            {
            if ( itC.key() == LoggerConfig )
                continue;
            
            if ( itC.value().loggers().contains( ObjectName ) )
                {
                Remove = false;
                break;
                }
            }
        
        if ( Remove && m_mapLogObj.contains( ObjectName ) )
            m_mapLogObj.remove(ObjectName);
        }
    
    m_LoggerConfigs.remove( LoggerConfig );
    }

void LoggerThread::removeLogObject( QString strObjName, QString LoggerConfig )
	{
	QMutexLocker lock(&m_ObjMutex);
	
	if ( !LoggerConfig.size() )
	{
	  for (QMap<QString, RLoggerConfig>::iterator it = m_LoggerConfigs.begin(); it != m_LoggerConfigs.end(); it++)
	      it.value().removeLogger(strObjName);
	  
	  if (m_mapLogObj.contains( strObjName ))
	      m_mapLogObj.remove(strObjName);
	  
	  return;   
	}
	
	if ( m_LoggerConfigs[LoggerConfig].loggers().contains( strObjName ) )
	    {
	    m_LoggerConfigs[LoggerConfig].removeLogger( strObjName );
	    
	    if ( !m_LoggerConfigs[LoggerConfig].loggers().size())
	        m_LoggerConfigs.remove(LoggerConfig);
	    }
	else
	    return;
	
	bool Remove = true;
	
	for (QMap<QString, RLoggerConfig>::iterator it = m_LoggerConfigs.begin(); it != m_LoggerConfigs.end(); it++)
	    if (it.value().loggers().contains(strObjName))
	        {
	        Remove = false;
	        break;
	        }

	if ( Remove && m_mapLogObj.contains( strObjName ) )
		m_mapLogObj.remove( strObjName );
	}


void LoggerThread::setConfigEnabled( QString LoggerConfig, bool Value )
    {
        QMutexLocker lock(&m_ObjMutex);
        
        if ( m_LoggerConfigs.contains(LoggerConfig) )
            m_LoggerConfigs[LoggerConfig].setEnabled(Value);
    }

void LoggerThread::setWithThreadname( QString LoggerConfig, bool Value )
    {
    QMutexLocker lock(&m_ObjMutex);
    
    if ( m_LoggerConfigs.contains(LoggerConfig) )
        m_LoggerConfigs[LoggerConfig].setWithThreadname(Value);
    }

void LoggerThread::setWithTimestamp( QString LoggerConfig, bool Value )
    {
    QMutexLocker lock(&m_ObjMutex);
    
    if ( m_LoggerConfigs.contains(LoggerConfig) )
        m_LoggerConfigs[LoggerConfig].setWithTimestamp(Value);
    }

void LoggerThread::run()
	{
	QString LogMessage, LoggerConfig;
	bool KillThread = false;
	QRegExp r("([a-zA-Z0-9]{1,}: )([0-9]{2}:[0-9]{2}:[0-9]{2}:[0-9]{3}: )(.*)");

#ifdef WIN32
	SetThreadAffinityMask(GetCurrentThread(), 1);
#endif
	
	for ( ;; )
	    {
	    m_pLogQ->receive( m_Data );
	    LogMessage = m_Data.data();
	    
	    if ( LogMessage == QString(LOGGER_KILL_COMMAND) )
	        {
	        LogMessage = QString("LoggerThread: Committing suicide");
	        LoggerConfig = QString(DEFAULT_LOGGER_CONFIG);
	        KillThread = true;
	        }
	    else
	        LoggerConfig = m_Data.loggerConfig();
	    
	    if ( !m_LoggerConfigs.contains(LoggerConfig) || !m_LoggerConfigs[LoggerConfig].enabled() ) continue;
        
        if (r.indexIn(LogMessage) != -1)
        {
            QString temp;
            
            if ( m_LoggerConfigs[LoggerConfig].withThreadname() )
                temp = temp + r.cap(1);
            
            if ( m_LoggerConfigs[LoggerConfig].withTimestamp() )
                temp = temp + r.cap(2);
            
            temp = temp + r.cap(3);
            
            LogMessage = temp;
        }
	    
	    const QStringList LoggerNames = m_LoggerConfigs[LoggerConfig].loggers();
	    
	    for (QStringList::const_iterator it = LoggerNames.begin(); it != LoggerNames.end(); it++)
	        if ( ILogObject* l = m_mapLogObj[*it] )
	            l->outputText(LogMessage);
	    
	    if ( KillThread ) return;
	    }
	}
/* --------------------------------------------------------------------*/
