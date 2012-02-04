
#ifndef __LOGGERLIB_H__
#define __LOGGERLIB_H__

// QT includes
#include <QtGlobal>
#include <QString>
#include <QMap>
#include <QThread>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QMutex>
#include <QFile>
#include <QStringList>

#include "perf_timer.h"
#include "rdefines.h"
#include "loghelper.h"


class LoggerThread;
class RLoggerData;
class QProcess;

template <class X> class RMsgQIntern;

// Defines
#ifdef LOG
#undef LOG
#endif

#define LOG(txt...) Logger::logPure(txt)
#define LOG_CFG(cfg, txt...) Logger::logPureCfg(QString(cfg), txt)
#define LOGL(slot, level, txt...) Logger::logLevel(slot, level, txt)
#define LOGL_CFG(cfg, slot, level, txt...) Logger::logLevelCfg(QString(cfg), slot, level, txt)
#define DLL_VERSION(name, revision) LoggerVersion logVersion(name, __FILE__, __DATE__, __TIME__, revision)
#define DLL_VERSION_CFG(cfg, name, revision) LoggerVersion logVersion(name, __FILE__, __DATE__, __TIME__, revision, cfg)
//#define FILE_VERSION(revision) static char szRevision[] = revision
#define FILE_VERSION(revision) 

/// Returns a \c QString as a \c const \c char *.
/// This is equivalent to \c qstring.toLocal8bit().constData().
#define QSTR_TO_ASCII qPrintable

class DLL_EXPORT LoggerVersion
	{
	public:
		LoggerVersion(const char* szLib, const char* szFile, const char* szDate, const char* szTime, const char* szRevision, const char *LoggerConfig = NULL);
		~LoggerVersion();
	};
	
	
class DLL_EXPORT ILogObject
	{
	public:
		ILogObject();
		virtual ~ILogObject();
		virtual void outputText( QString strText ) = 0;
	};


class DLL_EXPORT ConsoleLogger : public ILogObject
	{
	public:
		ConsoleLogger();
		virtual ~ConsoleLogger();

		virtual void outputText( QString strText );

	private:
		QTextStream m_qout;
	};


class DLL_EXPORT FileLogger : public ILogObject
	{
	public:
		FileLogger( QString strPath, bool bForceClean = false);
		virtual ~FileLogger();

		virtual void outputText( QString strText );
		
	private:
        void setLogFile(bool bForceClean = false);
        void cleanupLogFiles(int nKeepDays);

        QMutex m_hMutex;
        QFile m_logFile;
        QDate m_logFileDate;
        QString m_strLogRootDir;
	};

	

class DLL_EXPORT Logger
	{
	public:
		Logger( QString strSlot );
		Logger( QString strSlot, QString strFuncName );
		~Logger();

		static void logPure( QString strText, ... );
		static void logPureCfg( QString LoggerConfig, QString strText, ... );
		static void logLevel( QString strSlot, int nLevel, QString strText, ... );
		static void logLevelCfg( QString LoggerConfig, QString strSlot, int nLevel, QString strText, ... );
		
		void log( int nLevel, QString strText, ... );
		void logCfg( QString LoggerConfig, int Level, QString Text, ...);
		int getLogLevel();

		QString getExactOrigin( unsigned int unLine );
		QString getWarningText( QString strFuncName, unsigned int unLine );
		QString getErrorText( QString strFuncName, unsigned int unLine );
		QString getErrorTextCustom(QString strFuncName, unsigned int unLine);
		QString getInfoText(QString strFuncName, unsigned int unLine);
		QString getClassText(QString strFuncName, unsigned int unLine);
		
	private:
		static void format( QString& strText, va_list args );
		QString getSimplifiedFunctionName( QString& funcName );

		QString m_strSlot;
		QString m_strSimplifiedFuncName;
	};


class DLL_EXPORT LoggerSlot
	{
	public:
		LoggerSlot( QString strSlot, int nStartLogLevel = 0 );
		~LoggerSlot( );
		
	private:
		QString m_strSlot;
	};

/*!
    \class LoggerBase
    \brief Static logging class

    The main logging class. The class adds all needed functions to setup the complete logging loop.
    It supports multiple logging slots as well as different log levels.

    Usage:

    \code
int main( int argc, char ** argv )
    {
    ConsoleLogger console;      // create logging object
    FileLogger file(".");       // create another logging object
    file.setPath("/tmp/");  // setup log path for file logger
    FileLoggerEx file2("c:/test/"); // create a third logging object
    file2.setMaxFiles(-1);        // no limit for files in the archive
    file2.setMaxSize( 10 << 10 ); // set maximum file size to 10kB
    
    LoggerBase *lb = LoggerBase->get();

    lb->addLogObject("Console", &console);  // add logging object to LoggerBase
    lb->addLogObject("File", &file);    // add logging object to LoggerBase
    lb->addLogObject("File2", &file2, "NewLoggerConfig"); // add logging object to a specific logger config
    lb->setWithThreadname("NewLoggerConfig", false); // disable prefixing each log message with the name of the current thread for this logger config
    lb->setWithTimestamp("NewLoggerConfig", false); // disable timestamp for this logger config
    lb->start();                // start logging thread

    lb->addSlot("TestApp",1);       // add a logging slot
    lb->addSlot("Debug",0);         // add another logging slot (for debugging)
    
    ...
    
    LOG_ENTRY;
    LOG("Hello to default config");   // this goes to default logger config
    LOG_CFG("NewLoggerConfig", "Hello to NewLoggerConfig"); // this goes to the logger config named "NewLoggerConfig"
    
    ...
    
    lb->setConfigEnabled("NewLoggerConfig", false); // disable logger config "NewLoggerConfig"    
    LOG_CFG("NewLoggerConfig", "This line won't appear in the File2's log, because the config is disabled!");
    lb->setConfigEnabled("NewLoggerConfig", true); // reenable the logger config
    
    ...
    
    lb->removeLoggerConfig("NewLoggerConfig");  // remove the logger config
    LOG_CFG("NewLoggerConfig", "this line won't appear either, because the logger config doesn't exist anymore!");

    ...
    ...

    lb->setLogLevel("Debug",5);     // set debug slot to level 5
    
    Logger logger("Debug");             // define own logger object

    logger.log(1,"Define your own log text here");
    logger.logCfg("NewLoggerConfig", 1, "Another fancy log text");  // this line will only appear in the loggers, which belong to logger config "NewLoggerConfig".

    LoggerBase::setLogLevel("Debug",5);     // set debug slot to level 5

    ...
    ...
    ...
    }
    \endcode

 */
	
class DLL_EXPORT LoggerBase
	{
	public:
	    /// Getter for the global LoggerBase- instance.
	    /// \return pointer to the global LoggerBase instance
		static LoggerBase* get();
		
		/// Getter for a named logging object 
		/// \param[in] ObjectName determines the name of the logging object to retrieve
		/// \return pointer to the logging object which is registered under the name \c ObjectName
		ILogObject* getLogObject( QString ObjectName );
		
		/// adds a (new) logging object pointet to by \c pLogObj \c strObjName to the logger configuration \c LoggerConfig. 
		/// If no logger config is specified, the object is added to the default logger config.
		/// Log object can be added multiple times to one and the same or different logger configs.
		/// \param[in] strObjName represents the name under which the logging object should be added
		/// \param[in] pLogObj is a pointer to the logging object
		/// \param[in] LoggerConfig is the destination logger config, the new logger object should appear in. If the config doesn't exist yet, it is created.
		/// \sa removeLogObject
		void addLogObject( QString strObjName, ILogObject* pLogObj, QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		
		/// removes a logger configuration.
		/// \param[in] LoggerConfigs is the name of the configuration which should be removed.
		/// \sa addLogObject
		void removeLoggerConfig( QString LoggerConfig );
		
		/// removes a logging object.
		/// \param[in] strObjName name of the logging object.
		/// \param[in] LoggerConfig the name of the logger config the logging object should be remove from.
		/// By default (no \c LoggerConfig argument is given) the logging object is remove from the default logger config.
		/// If you pass an empty string, the logging object is remove from all registered logger configurations.
		/// \sa addLogObject
		void removeLogObject( QString strObjName, QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		
		/// registers a logging slot
		/// \param[in] strSlot is the name of the slot
		/// \param[in] nDefaultLevel is the default logging level the slot is initialized with
		void addSlot( QString strSlot, int nDefaultLevel = 0 );
		
		/// removes a logging slot
		/// \param[in] strSlot is the name of the slot to be removed
		void removeSlot( QString strSlot );
		
		/// starts the logging loop (actually starts a thread)
		void start();
		
		/// stops the logging loop (actually forces the logging- thread to kill itself)
		void stop();
		
		/// Getter for logger configuration \c LoggerConfig's \c ConfigEnabled property. Queries whether or not the logger config \c LoggerConfig is in enabled state.
		/// \param[in] LoggerConfig is the name of the logger config in whose state we are interested in.
		/// \return \c true if the config is enabled, \c false otherwise.
		/// \sa setConfigEnabled
		bool configEnabled( QString LoggerConfig );
		
		/// Getter for the \c Enabled property. Is the logging loop running?
		/// \return \c true if the logger loop is running; \c false otherwise
		/// \sa start, stop
		bool enabled();
		
		/// Getter for logger configuration \c LoggerConfig's \c WithThreadname property which specifies whether or not 
		/// each log line sent to a certain logger config is prefixed by the name of the calling thread. 
		/// \param[in] LoggerConfig specifies the name of the logger config. If you omit this argument the status of the default logger config is queried.
		/// \return \c true if the \c WithThreadname flag is set for config \c LoggerConfig, \c false otherwise. 
		/// \sa withTimestamp, setWithThreadname, setWithTimestamp		
		bool withThreadname( QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		
		/// Getter for logger config \c LoggerConfig's \c WithTimestamp property which determines if each log line sent 
		/// to a certain logger config should be prefixed by the current timestamp.
		/// \param[in] LoggerConfig specifies the name of the logger config. Default is the identifier of the default logger config.
		/// \return \c true if the \c Withtimestamp flag is set for config \c LoggerConfig, \c false otherwise.
		/// \sa withThreadname, setWithThreadname, setWithTimestamp
		bool withTimestamp( QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		
		/// lists all registered logging slots to the internal instance of ConsoleLogger.
		void listSlots();
		
		/// returns a \c QMap which maps the names of the registered logging slots to their current logging levels
		/// \return a map containing the logging levels
		QMap<QString, int> getSlots();
		
		/// sets the logging level of logging slot \c strSlot to level \c nLevel.
		/// \param[in] strSlot is the name of the slot whose level we want to update
		/// \param[in] nLevel the new logging level for the slot
		void setLogLevel( QString strSlot, int nLevel );
		
		/// retrieves the logging level set for logging slot \c strSlot
		/// \param[in] strSlot represents the name of the logging we are interested in.
		/// \return the logging level currently set for logging slot \c strSlot.
		int getLogLevel( QString strSlot );

		/// logs to a certain logger config, using a specific logging slot and level
		/// \param[in] LoggerConfig is the logger config we want to access
		/// \param[in] strSlot represents the logger slot 
		/// \param[in] nLevel specifies the log message's level
		/// \param[in] strText is the log message we want to write
		void log( QString LoggerConfig, QString strSlot, int nLevel, QString strText );
		
		/// similar to the previous function; logs the message \c strText to the logger config \c LoggerConfig.
		/// \param[in] LoggerConfig is the destination logger configuration
		/// \param[in] strText is the log message we want to save.
		void log( QString LoggerConfig, QString strText );

		/// returns the current timestamp and forces a reinitialization of the timing mechanism once an hour
		/// \return timestamp; format: hh:mm:ss:zzz
		/// \sa reinitTimer
        QString currentTime();
        
        /// reinitializes the internal timing mechanism consisting of an initial time an offset created/countet by a timer.
        /// \sa currentTime
        void reinitTimer();
        
        /// Setter for logger configuration \c LoggerConfig's \c ConfigEnabled property. Enables or disables a logger configuration.
        /// \param[in] LoggerConfig is the name of the logger config whose state should be changed
        /// \param[in] Value specifies if the config is enabled or disabled.
        /// \sa setWithTreadname, setWithTimestamp, configEnabled  
        void setConfigEnabled( QString LoggerConfig, bool Value = true );
        
        /// Setter for \c LoggerConfig's \c WithThreadname property. Enables or disables prefixing loglines with the name of the current thread.
        /// \param[in] LoggerConfig is the name of the logger configuration whose \c WithThreadname property we want to change
        /// \param[in] Value is the new value.
        /// \sa setConfigEnabled, setWithTimestamp, withThreadname, withTimestamp
        void setWithThreadname( QString LoggerConfig, bool Value = true );
        
        /// Setter for \c LoggerConfig's \c WithThreadname property. Enables or disables prefixing loglines with the name of the current thread.
        /// \param[in] LoggerConfig is the name of the logger configuration whose \c withTimestamp property we want to change
        /// \param[in] Value is the new value.
        /// \sa setConfigEnabled, setWithThreadname, withThreadname, withTimestamp
        void setWithTimestamp( QString LoggerConfig, bool value = true );

	private:
	    /// an internal logging mechanism. Writes text \c str to an internal instance of ConsoleLogger.
	    /// \param[in] str the thes which we want to output.
	    /// \sa console
		void logIntern(QString str);		
		
		/// ctr;
		/// we keep this private in order to ensure the singleton- pattern (only one global instance which is accessed by a getter).
		/// \sa get
		LoggerBase();
		
		/// dtr
		~LoggerBase();
		
	private:
		static LoggerBase* m_pBase;         ///< one global instance of LoggerBase
		
        QTime m_initialTime;                ///< initialization time. When was the logging system initialized the last time? \sa reinitTimer, currentTime
        PerformanceTimer m_perfTimer;       ///< used to perform a reinitialization every hour

		QMap<QString, int> m_mapSlots;     ///< maps the logging slots (names) to their currently set logging levels
		LoggerThread* m_pLoggerThread;     ///< pointer to a LoggerThread object, which implements the logging loop
		RMsgQIntern<RLoggerData>* m_pLogQ; ///< pointer to a message queue used for passing the log events to the logger thread.
		QMutex m_SlotMutex;                ///< used for synching critical method
		ConsoleLogger console;             ///< internal console logger. \sa logIntern
		bool m_bLogging;                   ///< is the logger loop running? \sa start, stop, enabled
	};
	
class DLL_EXPORT RLoggerConfig
    {
        private:
            bool m_Enabled,         ///< is the logger config currently enabled? \sa enabled, setEnabled 
                 m_WithThreadname,  ///< should each line be prefixed with with the name of the current thread? \sa withThreadname, setWithThreadname
                 m_WithTimestamp;   ///< should a timestamp be added to each line? \sa withTimestamp, setWithTimestamp
            
            QString m_Name;        ///< name of the logger config; must be unique! \sa setName, name
            QStringList m_Loggers; ///< list of (names of) loggers which belong to the logger configuration. \sa addLogger, removeLogger, loggers
            
        public:
            /// standard ctr
            RLoggerConfig();
            
            /// overloaded ctr. initializes the \c name property
            RLoggerConfig( QString Name );
            
            /// dtr.
            virtual ~RLoggerConfig();
            
            /// getter for the \c Enabled property. Tells the logger thread whether it should process or drop incomming log messages for this configuration.
            /// \return \c true if the logger configuration is enabled, \c false otherwise
            virtual bool enabled();
            
            /// getter for the \c WithThreadname property. Determines, whether or not each log message should be prefixed with the name of the current thread.
            /// \return \c true if the \c WithThreadname property is enalbed for the configuration, \c false otherwise
            /// \sa setWithThreadname, withTimestamp, m_WithTimestamp
            virtual bool withThreadname();
            
            /// getter for the \c WithTimestamp property. Determines, whether or not each log message should be prefixed with the current timestamp.
            /// \return \c true if the \c WithTimestamp property is enalbed for the configuration, \c false otherwise
            /// \sa setWithTimestamp, withThreadname, m_WithThreadname
            virtual bool withTimestamp();
            
            /// getter for the \c Name propoerty.
            /// /return the name of the logger config.
            /// \sa setName
            virtual QString name();
            
            /// returns a list, which contains all loggers that belong to the config.
            /// \return a \c QStringList with the names of the logging objects
            /// \sa addLogger, removeLogger, m_Loggers
            virtual const QStringList& loggers();
            
            /// setter for the \c Enabled property.
            /// \param[in] Value is the new value.
            /// \sa enabled
            virtual void setEnabled( bool Value = true );
            
            /// setter for the \c WithThreadname property.
            /// \param[in] Value is the new Value.
            /// \sa withThraedname
            virtual void setWithThreadname( bool Value = true );
            
            /// setter for the \c WithTimestamp property.
            /// \param[in] Value is the new Value.
            /// \sa withTimestamp
            virtual void setWithTimestamp( bool Value = true );
            
            /// adds a new logger (the name of a logger object) to the configuration.
            /// \param[in] Name is the name of the new logger object.
            /// \sa removeLogger, loggers
            virtual void addLogger( QString Name );
            
            /// removes a logger previously added with \c addLogger from of the configuration.
            /// \param[in] Name is the name of the logger object to be removed
            /// \sa addLogger, loggers
            virtual void removeLogger( QString Name );
    };


#endif // __LOGGERLIB_H__
