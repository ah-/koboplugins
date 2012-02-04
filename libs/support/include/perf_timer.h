#ifndef __PERF_TIMER_H__
#define __PERF_TIMER_H__

// QT Includes
#include <QMutex>
#include <QMap>
#include <QVector>

// Includes
#include "rdefines.h"

#include <sys/time.h>
#include <time.h>

// Defines


/**************************************************************************************/

class DLL_EXPORT PerformanceTimer
	{
	public:
		PerformanceTimer();
		~PerformanceTimer();

		void init();
		double elapsed(bool bReset = false);
		static void sleep(long lMsec);
		static void nsleep(long lNanoSec);

	private:
        struct timeval m_lastTime;
	};

/**************************************************************************************/

class DLL_EXPORT RTimeout
	{
	public:
		RTimeout(int ms);
		~RTimeout();

		bool timeout();
		void reset();
		void set(int ms);

	private:
		int m_nMs;
		PerformanceTimer timer;
	};


/**************************************************************************************/

#endif // __PERF_TIMER_H__
