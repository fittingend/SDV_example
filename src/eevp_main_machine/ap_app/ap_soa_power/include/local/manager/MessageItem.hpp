#ifndef __MESSAGE_ITEM_HPP__
#define __MESSAGE_ITEM_HPP__

#include <Common.h>

#include <atomic>
#include <functional>
#include <mutex>

typedef std::function<int(int, int, int, int, void *)> MsgFunc_t;

typedef struct funcMsgItem {
	MsgFunc_t mFunc;
	int mArg1;
	int mArg2;
	int mArg3;
	int mArg4;
	void *mArgPtr;
	std::function<void(void *)> mFree;
	std::mutex *mMutexForModal;
	int *mRetPtr;

	funcMsgItem()
	{
		mArg1 = 0;
		mArg2 = 0;
		mArg3 = 0;
		mArg4 = 0;
		mArgPtr = nullptr;
		mFunc = nullptr;
		mFree = nullptr;
		mMutexForModal = nullptr;
		mRetPtr = nullptr;
	}
} FuncMsgItem_t;

#endif