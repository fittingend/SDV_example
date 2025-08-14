#include <Common.h>
#include <MessageQueue.hpp>
#include <iostream>
#include <thread>
#include <list>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <Log.hpp>

#include <ManagerBase.hpp>

//#define USE_MUTEX_QUEUE

std::mutex ManagerBase::s_MutexForManagerList;
std::list<ManagerBase *> ManagerBase::s_ManagerList;

ManagerBase::ManagerBase()
{
	std::unique_lock<std::mutex> lock(ManagerBase::s_MutexForManagerList);

	this->mThreadRunning = false;
	this->mThread = std::thread(&ManagerBase::runThread, this);
	ManagerBase::s_ManagerList.push_back(this);
}

ManagerBase::~ManagerBase()
{
	std::unique_lock<std::mutex> lock(ManagerBase::s_MutexForManagerList);
	if (this->mThreadRunning)
	{
		this->Stop();
	}
	// std::unique_lock<std::mutex> lock(ManagerBase::s_MutexForManagerList);
	ManagerBase::s_ManagerList.remove(this);
}

std::mutex *ManagerBase::getMutexForModalApi()
{
#if defined(USE_MUTEX_QUEUE)
	{
		std::unique_lock<std::mutex> lock(this->mMutexForMtxQueue);
		if (this->mMutexQueue.empty())
		{
			return new std::mutex();
		}
		else
		{
			std::mutex *ret = this->mMutexQueue.front();
			this->mMutexQueue.pop();
			return ret;
		}
	}
#else
	{
		return new std::mutex();
	}
#endif
}

void ManagerBase::releaseMutexForModalApi(std::mutex *mtx)
{
#if defined(USE_MUTEX_QUEUE)
	{
		std::unique_lock<std::mutex> lock(this->mMutexForMtxQueue);
		this->mMutexQueue.push(mtx);
	}
#else
	{
		delete mtx;
	}
#endif
}

void ManagerBase::runThread()
{
	this->mThreadRunning = true;
	while (this->mThreadRunning)
	{
		FuncMsgItem_t msgItem = this->mMsgQueue.Pop();
		if (msgItem.mFunc != nullptr)
		{
			int retVal = msgItem.mFunc(msgItem.mArg1, msgItem.mArg2, msgItem.mArg3, msgItem.mArg4, msgItem.mArgPtr);
			if (msgItem.mRetPtr != nullptr)
			{
				*(msgItem.mRetPtr) = retVal;
			}
		}

		if (msgItem.mArgPtr != nullptr && msgItem.mFree != nullptr)
		{
			msgItem.mFree(msgItem.mArgPtr);
		}

		if (msgItem.mMutexForModal != nullptr)
		{
			msgItem.mMutexForModal->unlock();
		}
	}

	if (this->mPostThreadStopFunc != nullptr)
	{
		this->mPostThreadStopFunc();
	}
}

int ManagerBase::sendMessage(FuncMsgItem_t msgItem, bool isModal)
{
	std::mutex *modal = isModal ? this->getMutexForModalApi() : nullptr;
	int retVal = 0;

	if (isModal)
	{
		modal->lock();
		msgItem.mMutexForModal = modal;
		msgItem.mRetPtr = &retVal;
	}

	this->mMsgQueue.Push(msgItem);

	if (isModal)
	{
		modal->lock();
		modal->unlock();
		this->releaseMutexForModalApi(modal);
	}

	return retVal;
}

int ManagerBase::executeFunction(MsgFunc_t func, int arg1, int arg2, int arg3, int arg4, void *argv)
{
	FuncMsgItem_t msgItem;
	msgItem.mFunc = func;
	msgItem.mArg1 = arg1;
	msgItem.mArg2 = arg2;
	msgItem.mArg3 = arg3;
	msgItem.mArg4 = arg4;
	msgItem.mArgPtr = argv;

	this->sendMessage(msgItem);
	return 0;
}

int ManagerBase::Stop()
{
	this->mThreadRunning = false;
	FuncMsgItem_t dummyMsg;
	this->sendMessage(dummyMsg);
	this->mThread.join();
	return 0;
}

void ManagerBase::RegisterPostStopFunction(std::function<void(void)> func)
{
	this->mPostThreadStopFunc = func;
}

void ManagerBase::StopAllManagers()
{
	std::unique_lock<std::mutex> lock(ManagerBase::s_MutexForManagerList);

	for (ManagerBase *mgr : ManagerBase::s_ManagerList)
	{
		mgr->Stop();
	}
}