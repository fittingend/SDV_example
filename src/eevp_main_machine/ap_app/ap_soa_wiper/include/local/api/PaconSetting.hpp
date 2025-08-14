#ifndef __PACON_SETTING_HPP__
#define __PACON_SETTING_HPP__

#include <atomic>
#include <thread>
#include <Common.h>
#include <SoaDataType.h>
#include <MessageQueue.hpp>

class PaconSetting
{
private:
	static PaconSetting *mInst;

	bool mCoreInitialized;
	bool mExecutionStateReported;
	std::atomic<bool> mThreadRunning;
	std::thread *mPoolingThread;
	MessageQueue<int> mQueueForThreadStopping;

	PaconSetting();
	~PaconSetting();

	void poolingForUpdate();

public:
	static PaconSetting *GetInstance();
	static void RemoveInstance();

	bool StartPacon();
};

#endif