#include <Sleeper.hpp>

Sleeper::Sleeper()
{
	this->mEventForWakeup = false;
}

Sleeper::~Sleeper()
{
	this->WakeUp();
}

Sleeper::SleepResult_e Sleeper::Sleep()
{
	std::unique_lock<std::mutex> lock(this->mMutexForSleep);
	this->mEventForWakeup = false;
	this->mCvForSleeping.wait(lock, [this]{ return this->mEventForWakeup; });
	return Sleeper::eRESULT_EVENT_WAKEUP;
}

template<class Rep, class Period>
Sleeper::SleepResult_e Sleeper::SleepFor(const std::chrono::duration<Rep, Period>& rel_time)
{
	std::unique_lock<std::mutex> lock(this->mMutexForSleep);
	this->mEventForWakeup = false;
	if (this->mCvForSleeping.wait_for(lock, rel_time, [this]{ return this->mEventForWakeup; }) == std::cv_status::timeout)
	{
		return Sleeper::eRESULT_TIMEOUT;
	}

	return Sleeper::eRESULT_EVENT_WAKEUP;
}

void Sleeper::WakeUp()
{
	std::unique_lock<std::mutex> lock(this->mMutexForSleep);
	this->mEventForWakeup = true;
	this->mCvForSleeping.notify_one();
}