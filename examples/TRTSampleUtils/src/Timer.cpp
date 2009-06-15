//=====================================================================================================================
//
//   Timer.cpp
//
//   Simple timer class for benchmarking
//
//   Part of the TinyRT Raytracing Library.
//   Author: Joshua Barczak
//
//   Copyright 2009 Joshua Barczak.  All rights reserved.
//   See  Doc/LICENSE.txt for terms and conditions.
//
//=====================================================================================================================

#include "Timer.h"

#ifdef WIN32

    // #define this or it breaks
    #define WIN32_LEAN_AND_MEAN

    // on windows, we must use the win32 API for micro-second timing
    #include <windows.h>

#else
    // on non-windows, time.h should do the job.
    #include <time.h>

    // check in case we port to some platform without enough precision
    #if (CLOCKS_PER_SEC < 1000000)
        #error "CLOCKS_PER_SEC is too low for micro-second precision.  Do something about it."
    #endif

#endif



// on windows, we can't use clock() because it has only millisecond precision
// so we use their high performance counter API instead.  Rather annoying.
#ifdef WIN32

    static bool s_freq_queried=false;
    static LARGE_INTEGER s_freq;

    class Win32Timer : public Timer::TimerImpl
    {
        LARGE_INTEGER m_start;
    public:
        
        Win32Timer()
        {
            if(!s_freq_queried)
            {
                QueryPerformanceFrequency(&s_freq);
                s_freq_queried=true;
            }
            QueryPerformanceCounter(&m_start);
        }

        unsigned int Tick() const
        { 
            LARGE_INTEGER now;
            QueryPerformanceCounter( &now );
            __int64 start = (m_start.QuadPart ) ;
            __int64 ticks_per_second = s_freq.QuadPart;
            __int64 ticks = now.QuadPart - start;
            return (unsigned int) (ticks / (ticks_per_second/1000));
            return 0;
        }

        unsigned long TickMicroSeconds() const
        {
            LARGE_INTEGER now;
            QueryPerformanceCounter( &now );
            //__int64 start = (m_start.QuadPart) ;
            __int64 ticks_per_second = s_freq.QuadPart;
            __int64 ticks_per_usecond = ticks_per_second/1000000;
            __int64 ticks = now.QuadPart - m_start.QuadPart;
            return (unsigned long) (ticks / (ticks_per_usecond));
            return 0;
        }

        void Reset()
        {
            QueryPerformanceCounter(&m_start);
        }

    };


#else

    // UNIX version can use clock() for everything since they have better precision

    class UnixTimer : public Timer::TimerImpl
    {
        clock_t m_start;
    public:

        UnixTimer() : m_start(clock()) {};
    
        unsigned int Tick() const
        {
            clock_t elapsed = clock() - m_start;
            elapsed /= ((CLOCKS_PER_SEC)/1000);
            return (int)elapsed;
        }

        unsigned long TickMicroSeconds() const
        {
            clock_t elapsed = clock() - m_start;
            elapsed /= ((CLOCKS_PER_SEC)/1000000);
            return (int)elapsed;
        }

        void Reset()
        {
            m_start = clock();
        }
    };

#endif


Timer::Timer()
{
#ifdef WIN32
    m_pImpl = new Win32Timer();
#else
    m_pImpl = new UnixTimer();
#endif
}

Timer::~Timer()
{
    delete m_pImpl;
}