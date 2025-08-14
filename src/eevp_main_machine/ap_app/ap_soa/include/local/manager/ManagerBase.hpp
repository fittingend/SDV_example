#ifndef __MANAGER_BASE_HPP__
#define __MANAGER_BASE_HPP__

#include <Common.h>

#include <MessageQueue.hpp>
#include <MessageItem.hpp>
#include <thread>
#include <atomic>
#include <list>
#include <mutex>

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

    void runThread();
    int sendMessage(FuncMsgItem_t msgItem, bool isModal = false);

    ManagerBase();
    ~ManagerBase();

    std::mutex *getMutexForModalApi();
    void releaseMutexForModalApi(std::mutex *mtx);


public:
    int Stop();
    static void StopAllManagers();

};
#endif