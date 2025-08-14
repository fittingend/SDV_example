#ifndef __SIMPLE_TIMER_ITEM_HPP__
#define __SIMPLE_TIMER_ITEM_HPP__

#include <mutex>
#include <list>
#include <atomic>
#include <memory>
#include <chrono>
#include <thread>
#include <functional>
#include <Common.h>

namespace util {
namespace timer {

class SimpleTimerItem
{
private:
	static std::mutex mMutexForNextKey;
	static uint64_t s_mNextKeyValue;

protected:
	bool mFlagAvailable;
	bool mFlagToBeDeleted;
	bool mFlagInTimer;
	std::uint64_t mKey;
	std::uint64_t mTickTime;
	std::chrono::time_point<std::chrono::steady_clock> mStartTime;
	std::string mDebugText;

	std::function<void(SimpleTimerItem *)> mRunFunc;

public:
	SimpleTimerItem();
	virtual ~SimpleTimerItem();

	bool IsAvailable();
	void SetAvaiable(bool flag);

	bool IsTobeDeleted();
	void SetTobeDeleted(bool flag);

	bool IsInTimer();
	void SetThisIsInTimer(bool flag);

	std::uint64_t GetKey();

	std::uint64_t GetDuration();
	void SetDuration(std::uint64_t tick);

	std::string GetDebugText();
	void SetDebugText(std::string dbgText);

	std::chrono::time_point<std::chrono::steady_clock> GetStartTime();
	void SetStartTime();

	void SetFunction(std::function<void(SimpleTimerItem *)> func);
	void Run();
};


}
}


#endif // __SIMPLE_TIMER_ITEM_HPP__