#ifndef __MANAGER_BASE_HPP__
#define __MANAGER_BASE_HPP__

#include <Common.h>

#include <MessageQueue.hpp>
#include <MessageItem.hpp>
#include <thread>
#include <atomic>
#include <list>
#include <mutex>
#include <functional>

class ManagerBase
{
private:
	static std::mutex s_MutexForManagerList;
	static std::list<ManagerBase *> s_ManagerList;

protected:
	MessageQueue<FuncMsgItem_t> mMsgQueue;
	std::queue<std::mutex *> mMutexQueue;
	std::mutex mMutexForMtxQueue;
	std::atomic<bool> mThreadRunning;
	std::thread mThread;
	std::function<void(void)> mPostThreadStopFunc;

	void runThread();
	int sendMessage(FuncMsgItem_t msgItem, bool isModal = false);
	int executeFunction(MsgFunc_t func, int arg1 = 0, int arg2 = 0, int arg3 = 0, int arg4 = 0, void *argv = nullptr);

	ManagerBase();
	~ManagerBase();

	std::mutex *getMutexForModalApi();
	void releaseMutexForModalApi(std::mutex *mtx);

public:
	int Stop();
	void RegisterPostStopFunction(std::function<void(void)> func);
	static void StopAllManagers();

};
#endif
