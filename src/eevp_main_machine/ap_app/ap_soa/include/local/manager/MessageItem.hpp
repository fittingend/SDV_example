#ifndef __MESSAGE_ITEM_HPP__
#define __MESSAGE_ITEM_HPP__

#include <Common.h>

#include <atomic>
#include <functional>
#include <mutex>
#include <MessageItem.hpp>

typedef struct funcMsgItem {
    std::function<int(int, int, int, void *)> mFunc;
    int mArg1;
    int mArg2;
    int mArg3;
    void *mArgPtr;
    std::function<void(void *)> mFree;
    std::mutex *mMutexForModal;
    int *mRetPtr;

    funcMsgItem()
    {
        mArg1 = 0;
        mArg2 = 0;
        mArg3 = 0;
        mArgPtr = nullptr;
        mFunc = nullptr;
        mFree = nullptr;
        mMutexForModal = nullptr;
        mRetPtr = nullptr;
    }
} FuncMsgItem_t;

#endif