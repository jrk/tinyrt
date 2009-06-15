//=====================================================================================================================
//
//   Timer.h
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

#ifndef _TIMER_H_
#define _TIMER_H_


class Timer
{
public:
    
    Timer();

    ~Timer();

    /// Returns the number of milliseconds elapsed since this object was created, or since the last reset
    inline unsigned int Tick() const { return m_pImpl->Tick(); };

    /// Same as tick, but in microseconds
    inline unsigned long TickMicroSeconds() const { return m_pImpl->TickMicroSeconds(); };

    /// Resets the timer
    inline void Reset() { return m_pImpl->Reset(); };

    // Windows and linux need to use different member types, so we use the PIMPL idiom here to avoid
    //   including that dastardly windows.h header on Win32 builds
    class TimerImpl
    {
    public:
       
        virtual unsigned int Tick() const = 0;
        virtual unsigned long TickMicroSeconds() const = 0;
        virtual void Reset() = 0;
    };


private:

   
    TimerImpl* m_pImpl;
};



#endif
