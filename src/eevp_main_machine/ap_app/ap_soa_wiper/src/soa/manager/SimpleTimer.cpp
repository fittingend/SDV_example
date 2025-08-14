#include <SimpleTimer.hpp>
#include <string>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <Log.hpp>

namespace util {
namespace timer {

//
//  SIMPLE TIMER
//

std::shared_ptr<SimpleTimer> SimpleTimer::s_mInst;

std::shared_ptr<SimpleTimer> SimpleTimer::GetInstance()
{
	if (SimpleTimer::s_mInst.get() == nullptr)
	{
		SimpleTimer::s_mInst = std::make_shared<SimpleTimer>();
		SimpleTimer::s_mInst->startTimer();
	}

	return SimpleTimer::s_mInst;
}

void SimpleTimer::DestoryInstance()
{
	if (SimpleTimer::s_mInst.get() != nullptr)
	{
		SimpleTimer::s_mInst->stopTimer();
		SimpleTimer::s_mInst.reset();
	}
}

void SimpleTimer::startTimer()
{
	this->mThreadRunning = true;
	this->mEventForWakeup = false;
	this->mTimerThread = new std::thread(&SimpleTimer::runThread, this);

}

void SimpleTimer::stopTimer()
{
	this->mThreadRunning = false;
	this->wakeUpTimerThread();
	this->mTimerThread->join();
}

void SimpleTimer::runThread()
{
	uint64_t sleep_time;
	while (this->mThreadRunning)
	{
		this->checkAndRunItems();
		this->arrangeItems();
		this->sleepTimer();
	}
}

void SimpleTimer::checkAndRunItems()
{
	std::lock_guard<std::mutex> lock(this->mMutexForTimerList);

	// Add Items to the maps
	{
		std::lock_guard<std::mutex> lock2(this->mMutexForAddedTimerList);

		for (SimpleTimerItem *item : this->mAddedTimerItemList)
		{
			LOG_DEBUG() << "added item = " << item->GetKey() << "\n";
			item->SetAvaiable(true);
			item->SetTobeDeleted(false);
			this->mTimerItemList.push_back(item);
		}

		this->mAddedTimerItemList.clear();
	}

	for (SimpleTimerItem *item : this->mTimerItemList)
	{
		if (item->IsAvailable() && !item->IsTobeDeleted())
		{
			std::chrono::time_point<std::chrono::steady_clock> currTime = std::chrono::steady_clock::now();
			if ((currTime >= item->GetStartTime()))
			{
				item->Run();
				item->SetAvaiable(false);
				item->SetTobeDeleted(true);
			}
		}
	}
}

void SimpleTimer::arrangeItems()
{
	std::lock_guard<std::mutex> lock(this->mMutexForTimerList);

	// Add Items to the maps
	{
		std::lock_guard<std::mutex> lock2(this->mMutexForAddedTimerList);

		for (SimpleTimerItem *item : this->mAddedTimerItemList)
		{
			item->SetAvaiable(true);
			item->SetTobeDeleted(false);
			this->mTimerItemList.push_back(item);
		}

		this->mAddedTimerItemList.clear();
	}

	// Set the delete flag to the items
	{
		std::lock_guard<std::mutex> lock2(this->mMutexForToBeDeletedList);

		for (uint64_t key_remove : this->mToBeDeletedItemList)
		{
			for (SimpleTimerItem *item : this->mTimerItemList)
			{
				if (key_remove == item->GetKey())
				{
					item->SetTobeDeleted(true);
				}
			}
		}

		this->mToBeDeletedItemList.clear();
	}

	// Remove the flagged items
	for (std::list<SimpleTimerItem *>::iterator it = this->mTimerItemList.begin(); it != this->mTimerItemList.end();)
	{
		SimpleTimerItem *item = *it;
		if ((item) && (item->IsTobeDeleted()))
		{
			LOG_DEBUG() << "key " << item->GetKey() << " will be deleted... \n";
			it = this->mTimerItemList.erase(it);
			delete item;
		}
		else
		{
			it++;
		}
	}
}

void SimpleTimer::wakeUpTimerThread()
{
	std::unique_lock<std::mutex> lock(this->mMutexForSleep);
	this->mEventForWakeup = true;
	this->mCvForSleeping.notify_one();
}

void SimpleTimer::sleepTimer()
{
	uint64_t next_time = SimpleTimer::getNextWakeupTime();
	LOG_DEBUG() << "next sleep time = " << next_time << "ms\n";
	if (next_time > 0)
	{
		std::unique_lock<std::mutex> lock(this->mMutexForSleep);
		this->mEventForWakeup = false;
		this->mCvForSleeping.wait_for(lock, std::chrono::milliseconds(next_time), [this]{ return this->mEventForWakeup; });
	}
}

uint64_t SimpleTimer::getNextWakeupTime()
{
	uint64_t next_time = (1000 * 60 * 10);	   // 10min -> ms
	std::lock_guard<std::mutex> lock(this->mMutexForTimerList);
	std::chrono::time_point<std::chrono::steady_clock> curr_time = std::chrono::steady_clock::now();
	for (SimpleTimerItem *item : this->mTimerItemList)
	{
		if (item->IsAvailable())
		{
			if (curr_time >= item->GetStartTime())
			{
				return 0;
			}
			else
			{
				auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(item->GetStartTime() - curr_time);
				uint64_t diff_time = static_cast<uint64_t>(duration.count());
				if (diff_time < next_time)
				{
					next_time = diff_time;
				}
			}
		}
	}

	if (next_time < 5)
	{
		next_time = 5;
	}

	return next_time;
}

// Public Functions

void SimpleTimer::InsertItem(SimpleTimerItem *item)
{
	std::lock_guard<std::mutex> lock(this->mMutexForAddedTimerList);
	this->mAddedTimerItemList.push_back(item);
	item->SetStartTime();
	this->wakeUpTimerThread();
}

void SimpleTimer::DeleteItem(std::uint64_t key)
{
	std::lock_guard<std::mutex> lock(this->mMutexForToBeDeletedList);
	this->mToBeDeletedItemList.push_back(key);
	this->wakeUpTimerThread();
}

static std::string GetTimeString(std::chrono::time_point<std::chrono::steady_clock> time)
{
	std::stringstream ss;
	auto currunt_tick = std::chrono::steady_clock::now();
	auto currunt_clock = std::chrono::system_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time - currunt_tick);
	int diff_time = static_cast<int>(duration.count());
	auto start_clock = currunt_clock + std::chrono::milliseconds(diff_time);
	std::time_t now_time_t = std::chrono::system_clock::to_time_t(start_clock);
	std::tm local_time = *std::localtime(&now_time_t);

	ss << std::setw(4) << std::setfill('0') << (local_time.tm_year + 1900);
	ss << "/" << std::setw(2) << std::setfill('0') << (local_time.tm_mon + 1);
	ss << "/" << std::setw(2) << std::setfill('0') << (local_time.tm_mday);
	ss << " " << std::setw(2) << std::setfill('0') << (local_time.tm_hour);
	ss << ":" << std::setw(2) << std::setfill('0') << (local_time.tm_min);
	ss << ":" << std::setw(2) << std::setfill('0') << (local_time.tm_sec);

	return ss.str();
}

std::string SimpleTimer::GetTimerStatus()
{
	std::stringstream ss;
	std::lock_guard<std::mutex> lock(this->mMutexForTimerList);

	ss << "current time: [" << GetTimeString(std::chrono::steady_clock::now()) <<  "]\r\n";
	ss << "timer item number = " << this->mAddedTimerItemList.size() << "\r\n";
	ss << "----------------------------------------------------------------------------------------------\r\n";
	for (SimpleTimerItem *item : this->mTimerItemList)
	{
		ss << "- item (" << (int)item->GetKey() << ") : available=" << ((item->IsAvailable() ? "YES" : "NO")) << " ";
		ss << "StartTime: [" << GetTimeString(item->GetStartTime()) << "]\r\n  DbgTxt=\"" << item->GetDebugText() << "\"\r\n";
	}

	return ss.str();
}

std::uint64_t SimpleTimer::GetSystemTick()
{
    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return ms;
}

}
}