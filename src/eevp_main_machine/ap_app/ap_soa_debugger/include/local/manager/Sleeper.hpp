#ifndef __SLEEPER_HPP__
#define __SLEEPER_HPP__

#include <mutex>
#include <condition_variable>
#include <chrono>

class Sleeper
{
    typedef enum
    {
        eRESULT_OK = 0,
        eRESULT_TIMEOUT,
        eRESULT_EVENT_WAKEUP,

        eRESULT_ENDOFCASE
    } SleepResult_e;

private:
    std::mutex mMutexForSleep;
    std::condition_variable mCvForSleeping;
    bool mEventForWakeup;

public:

    Sleeper();
    ~Sleeper();
    SleepResult_e Sleep();
    template<class Rep, class Period>
        SleepResult_e SleepFor(const std::chrono::duration<Rep, Period>& rel_time);
    void WakeUp();
};


#endif