#include <atomic>
//#include <memory>
#include <SimpleTimerItem.hpp>
#include <Log.hpp>

namespace util {
namespace timer {

//
//  SIMPLE ITEM
//

std::mutex SimpleTimerItem::mMutexForNextKey;
uint64_t SimpleTimerItem::s_mNextKeyValue = 1;

SimpleTimerItem::SimpleTimerItem()
{
    std::lock_guard<std::mutex> lock(SimpleTimerItem::mMutexForNextKey);

    this->mFlagAvailable = true;
    this->mFlagToBeDeleted = false;
    this->mKey = SimpleTimerItem::s_mNextKeyValue;
    this->mRunFunc = nullptr;
    SimpleTimerItem::s_mNextKeyValue++;
}

SimpleTimerItem::~SimpleTimerItem()
{

}

bool SimpleTimerItem::IsAvailable()
{
    return this->mFlagAvailable;
}

void SimpleTimerItem::SetAvaiable(bool flag)
{
    this->mFlagAvailable = flag;
}

bool SimpleTimerItem::IsTobeDeleted()
{
    return this->mFlagToBeDeleted;
}

void SimpleTimerItem::SetTobeDeleted(bool flag)
{
    this->mFlagToBeDeleted = flag;
}

bool SimpleTimerItem::IsInTimer()
{
    return this->mFlagInTimer;
}

void SimpleTimerItem::SetThisIsInTimer(bool flag)
{
    this->mFlagInTimer = flag;
}

std::uint64_t SimpleTimerItem::GetKey()
{
    return this->mKey;
}

std::uint64_t SimpleTimerItem::GetDuration()
{
    return this->mTickTime;
}

void SimpleTimerItem::SetDuration(std::uint64_t tick)
{
    this->mTickTime = tick;
}

std::string SimpleTimerItem::GetDebugText()
{
    return this->mDebugText;
}

void SimpleTimerItem::SetDebugText(std::string dbgText)
{
    this->mDebugText = dbgText;
}

std::chrono::time_point<std::chrono::steady_clock> SimpleTimerItem::GetStartTime()
{
    return this->mStartTime;
}

void SimpleTimerItem::SetStartTime()
{
    this->mStartTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(this->mTickTime);
}

void SimpleTimerItem::SetFunction(std::function<void(SimpleTimerItem *)> func)
{
    this->mRunFunc = func;
}

void SimpleTimerItem::Run()
{
    if (this->mRunFunc != nullptr)
    {
        this->mRunFunc(this);
    }
}



}
}