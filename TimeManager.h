#pragma once

#ifndef _TIME_MANAGER_H_
#define _TIME_MANAGER_H_

#ifdef WIN32   // Windows system specific
    #include <windows.h>
#else          // Unix based system specific
    #include <sys/time.h>
#endif


    class TimeManager
    {
    public:
        // CTOR
        //
        TimeManager();

        // DTOR
        //
        ~TimeManager();

        void start();
        void stop();
        double getElapsedTime();                // By default elapsed time is returned in seconds.
        double getElapsedTimeInSeconds();       // Returns elapsed time in seconds.
        double getElapsedTimeInMicroseconds();  // Returns elapsed time in microseconds.
        double getElapsedTimeInMilliseconds();  // Returns elapsed time in milliseconds.
    private:
        double m_startTimeInMicroSeconds;
        double m_endTimeInMicroSeconds;
        int    m_stopped;

#ifdef WIN32
        LARGE_INTEGER m_frequency;
        LARGE_INTEGER m_startCount;
        LARGE_INTEGER m_endCount;
#else
        timeval m_startCount;
        timeval m_endCount;
#endif
    };

#endif // _TIME_MANAGER_H_