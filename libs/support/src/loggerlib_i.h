#ifndef __LOGGERLIB_I_H__
#define __LOGGERLIB_I_H__

// includes
#include <QString>
#include <QMutex>
#include "loghelper.h"

template <class X> class RMsgQIntern;
class RThread;
class RLoggerConfig;

/*!
    \class RLoggerData
    \brief Encapsulates data passed to the logger loop/logger thread

    Represents the data passed to the logger thread's message queue.
    This includes a the log message itself (\c data) and the destination
    logger config (\c loggerConfig).
    
*/

class RLoggerData
    {
    private:
        QString m_Data,          ///< the log message; \sa data, setData
                m_LoggerConfig;  ///< the destination logger configuration; \sa loggerConfig, setLoggerConfig
    
    public:
        /// default ctr
        /// Initializes the \c LoggerConfig property with the default logger config and \c Data with a default message
        /// for better debugging
        RLoggerData();
        
        /// overloaded ctr. Initializes \c m_Data and m_LoggerConfig with \c Data and \c LoggerConfig
        /// \param[in] Data is the log message
        /// \param[in] LoggerConfig is the destination logger config
        /// \sa m_Data, m_LoggerConfig, RLoggerData
        RLoggerData(QString Data, QString LoggerConfig);
        
        /// dtr
        virtual ~RLoggerData();
    
        /// Getter for the \c Data property; this is the log message which we want to post to the logger thread
        /// \return the log message
        /// \sa setData, m_Data, loggerConfig
        virtual QString data();
        
        /// Getter for the \c LoggerConfig property; this is the destination logger config the log message should be added to.
        /// \return the logger configuration
        /// \sa setLoggerConfig, m_LoggerConfig, data
        virtual QString loggerConfig();
    
        /// Setter for the \c Data property;
        /// \param[in] Value is the log message we want to post to the logger thread
        /// \sa data, setLoggerConfig
        virtual void setData(QString Value);
        
        /// Setter for the \c LoggerConfig property
        /// \param[in] Value specifies the logger config the log message should appear in.
        /// \see loggerConfig, setData
        virtual void setLoggerConfig(QString Value);
    };

class LoggerThread : public QThread
	{
	public:
		LoggerThread();
		void run();
		
		bool configEnabled( QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		bool withThreadname( QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		bool withTimestamp( QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );

		ILogObject* getLogObject( QString ObjectName );
		
		void setMsgQ( RMsgQIntern<RLoggerData>* qLog );
		void addLogObject( QString strObjName, ILogObject* pLogObj, QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		void removeLoggerConfig(QString LoggerConfig);
		void removeLogObject( QString strObjName, QString LoggerConfig = QString(DEFAULT_LOGGER_CONFIG) );
		void setConfigEnabled( QString LoggerConfig, bool Value = true);
		void setWithThreadname( QString LoggerConfig, bool Value = true );
		void setWithTimestamp( QString LoggerConfig, bool Value = true );

	private:
		RLoggerData m_Data;
		QMap<QString, ILogObject*> m_mapLogObj;
		RMsgQIntern<RLoggerData>* m_pLogQ;
		QMutex m_ObjMutex;
		QMap<QString, RLoggerConfig> m_LoggerConfigs;
	};



#endif // __LOGGERLIB_I_H__
