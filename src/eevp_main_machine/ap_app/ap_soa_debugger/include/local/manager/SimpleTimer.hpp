#ifndef __SIMPLE_TIMER_HPP__
#define __SIMPLE_TIMER_HPP__

#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <list>
#include <atomic>
#include <memory>
#include <thread>
#include <string>
#include <Common.h>
#include <SimpleTimerItem.hpp>

namespace util {
namespace timer {

class SimpleTimer
{
private:
    static std::shared_ptr<SimpleTimer> s_mInst;

    std::mutex mMutexForTimerList;
    std::mutex mMutexForAddedTimerList;
    std::mutex mMutexForToBeDeletedList;

    std::mutex mMutexForSleep;
    std::condition_variable mCvForSleeping;
    bool mEventForWakeup;

    //std::unordered_map<uint64_t, SimpleTimerItem *> mTimerItemMap;
	std::list<SimpleTimerItem *> mTimerItemList;
    std::list<SimpleTimerItem *> mAddedTimerItemList;
    std::list<uint64_t> mToBeDeletedItemList;
    std::thread *mTimerThread;
    std::atomic<bool> mThreadRunning;

    void startTimer();
    void stopTimer();
    void runThread();
    void checkAndRunItems();
    void arrangeItems();
    void wakeUpTimerThread();
    void sleepTimer();
    uint64_t getNextWakeupTime();

public:
    static std::shared_ptr<SimpleTimer> GetInstance();
    static void DestoryInstance();

    void InsertItem(SimpleTimerItem *item);
    void DeleteItem(std::uint64_t key);

    std::string GetTimerStatus();
};

}
}


#endif // __SELF_TIMER_H__