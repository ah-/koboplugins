#ifndef __MSGQ_INTERN_H__
#define __MSGQ_INTERN_H__

// includes
#include <QMutex>
#include <QMutexLocker>
#include <QSemaphore>
#include <QList>

// Includes
#include "perf_timer.h"


#define NORMAL_PRIORITY	0
#define HIGH_PRIORITY	1

#define NO_WAIT			0
#define WAIT_FOREVER   -1


template <class X> class RMsgQIntern
	{
	public:
		RMsgQIntern( int nElems = 10 );
		~RMsgQIntern();

		bool send( X data, short prio = NORMAL_PRIORITY );
		bool receive( X& data, short timeout = WAIT_FOREVER );
		bool clear();

	private:
		struct MSG_Q_DATA
			{
			X MsgQData;
			};

		mutable QMutex m_hMutex;
		QSemaphore m_hSemaphore;
		QList<struct MSG_Q_DATA> m_hList;

		int m_nElemsInQ;
		int m_nMaxElems;
        int m_maxSize;
		struct MSG_Q_DATA m_pData;
	};


template <class X> RMsgQIntern<X>::RMsgQIntern( int nElems )
	{
	m_nMaxElems = nElems;
    m_maxSize = 0;
	clear();
	}


template <class X> RMsgQIntern<X>::~RMsgQIntern()
	{
	}


template <class X> bool RMsgQIntern<X>::send( X data, short prio )
	{
	QMutexLocker locker( &m_hMutex );

	m_pData.MsgQData = data;

	if( prio == HIGH_PRIORITY )
		m_hList.prepend( m_pData );
	else
		m_hList.append( m_pData );

	m_nElemsInQ++;
    m_maxSize = (m_maxSize > m_nElemsInQ) ? m_maxSize : m_nElemsInQ;

	m_hSemaphore.release();

	return true;
	}


template <class X> bool RMsgQIntern<X>::receive( X& data, short timeout )
	{
	if(timeout == NO_WAIT)
		{
		if(!m_hSemaphore.tryAcquire())
			return false;
		}
	else if(timeout == WAIT_FOREVER)
		{
		m_hSemaphore.acquire();
		}
	else
		{
		PerformanceTimer timer;
		bool bAquire;
		
		while( (!(bAquire = m_hSemaphore.tryAcquire())) && (timer.elapsed() < timeout) )
			PerformanceTimer::sleep(1);
		
		if(!bAquire)
			return false;	
		}
		
	QMutexLocker locker( &m_hMutex );

	if ( !m_hList.isEmpty() )
		{
		m_pData = m_hList.takeFirst();
		m_nElemsInQ--;
		data = m_pData.MsgQData;

        if (m_maxSize > 500 && m_nElemsInQ < 200)
        {
            QList<struct MSG_Q_DATA> tmpList = m_hList;
            m_hList.clear();
            m_hList = tmpList;
            // printf("DEBUG: shrunk from <%d> to <%d> elements\n", m_maxSize, m_nElemsInQ);
            m_maxSize = m_nElemsInQ;
        }

		return true;
		}

	return false;
	}


template <class X> bool RMsgQIntern<X>::clear()
	{
	QMutexLocker locker( &m_hMutex );

	m_hList.clear();
	m_nElemsInQ = 0;
    m_maxSize = 0;

	while ( m_hSemaphore.tryAcquire() )
	    ;

	return true;
	}

#endif // __MSGQ_INTERN_H__
