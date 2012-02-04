// Windows Includes
#include <sys/time.h> // gettimeofday
#include <time.h>     // gettimeofday

// QT Includes
#include <QThread>
#include <QMutexLocker>
#include <QCoreApplication>

#include <time.h>

// Includes
#include "perf_timer.h"

/**************************************************************************************/

PerformanceTimer::PerformanceTimer()
	{
	init();
	}

PerformanceTimer::~PerformanceTimer()
	{}

void PerformanceTimer::init()
	{
    gettimeofday(&m_lastTime, NULL);
	}


/// Berechnet den Zeitunterschied zwischen \a pX und \a pY in [ms]
static double
timeval_subtract(const struct timeval *pX, const struct timeval *pY)
{
    int d_sec = pX->tv_sec - pY->tv_sec;
    int d_usec = pX->tv_usec - pY->tv_usec;

    return 1000 * d_sec + d_usec / 1000.0;
}


double PerformanceTimer::elapsed(bool bReset)
	{
    double iTimeDiff;

    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    iTimeDiff = timeval_subtract(&currentTime, &m_lastTime);

    if (bReset)
        m_lastTime = currentTime;

    return iTimeDiff;
	}

void PerformanceTimer::sleep(long lMsec)
	{
    struct timespec t;
    if (lMsec >= 1000)
        {
        t.tv_sec = lMsec / 1000;
        t.tv_nsec = 1000000 * (lMsec % 1000); // restliche [ms] in [nanosec]
        }
    else
        {
        t.tv_sec = 0;
        t.tv_nsec = 1000000 * lMsec; // [ms] in [nanosec]
        }
    struct timespec rem;
    while (-1 == nanosleep(&t, &rem))
        {
        t = rem;
        }
	}


void PerformanceTimer::nsleep(long lNanoSec)
    {
    struct timespec t;
    if(lNanoSec >= 1000000)
        {
        t.tv_sec = lNanoSec / 1000000;
        t.tv_nsec = (lNanoSec % 1000000); // restliche [nanosec]
        }
    else
        {
        t.tv_sec = 0;
        t.tv_nsec = lNanoSec; // [nanosec]
        }
    struct timespec rem;
    while (-1 == nanosleep(&t, &rem))
        {
        t = rem;
        }
    }

/**************************************************************************************/

RTimeout::RTimeout(int ms) : m_nMs(ms)
	{}

RTimeout::~RTimeout()
	{}

bool RTimeout::timeout()
	{
	switch(m_nMs)
		{
		case WAIT_FOREVER:
			return false;
			break;
		case NO_WAIT:
			return true;
			break;
		default:
			if(timer.elapsed() >= m_nMs)
				return true;
			break;
		}

	return false;
	}

void RTimeout::set(int ms)
	{
	m_nMs = ms;
	reset();
	}

void RTimeout::reset()
	{
	timer.init();
	}


/**************************************************************************************/
