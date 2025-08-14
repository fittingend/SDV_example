#include <Common.h>
#include <SoaDataType.h>
#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"
#include <PaconSetting.hpp>
#include <Api_SoaRoa.hpp>
#include <Api_Zone2Roa.hpp>
#include <Log.hpp>

PaconSetting *PaconSetting::mInst = nullptr;

PaconSetting *PaconSetting::GetInstance()
{
	if (PaconSetting::mInst == nullptr)
	{
		PaconSetting::mInst = new PaconSetting();
	}

	return PaconSetting::mInst;
}

void PaconSetting::RemoveInstance()
{
	if (PaconSetting::mInst != nullptr)
	{
		delete PaconSetting::mInst;
		PaconSetting::mInst = nullptr;
	}
}

PaconSetting::PaconSetting()
{
	this->mCoreInitialized = false;
	this->mExecutionStateReported = false;
	this->mThreadRunning = false;
	this->mPoolingThread = nullptr;
}

PaconSetting::~PaconSetting()
{
	if (this->mPoolingThread != nullptr)
	{
		this->mThreadRunning = false;
		this->mQueueForThreadStopping.Push(1);
		this->mPoolingThread->join();
		delete this->mPoolingThread;
		this->mPoolingThread = nullptr;
	}

	Zone2_SOA_Term();
	SOA_ROA_Term();

	if (this->mCoreInitialized)
	{
		ara::core::Deinitialize();
	}
}

bool PaconSetting::StartPacon()
{
	LOG_INFO() << "(+)\n";
	LOG_DEBUG() << "ara::core::Initialize() (+)\n";
	ara::core::Initialize();
	this->mCoreInitialized = true;
	LOG_DEBUG() << "ara::core::Initialize() (-)\n";

	LOG_DEBUG() << "ara::exec::ExecutionClient::ReportExecutionState (+)\n";
	ara::exec::ExecutionClient execClient{};
	ara::core::Result<void> result = execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
	if (!result)
	{
		LOG_ERROR() << "(-) execClient.ReportExecutionState failed\n";
		return false;
	}
	this->mExecutionStateReported = true;
	LOG_DEBUG() << "ara::exec::ExecutionClient::ReportExecutionState (-)\n";

	SOA_ROA_Init();
	Zone2_SOA_Init();


	this->mThreadRunning = true;
	this->mPoolingThread = new std::thread(std::bind(&PaconSetting::poolingForUpdate, this));

	LOG_INFO() << "(-)\n";
	return true;
}

void PaconSetting::poolingForUpdate()
{
	if (this->mThreadRunning)
	{
		int dummy_msg;

		this->mQueueForThreadStopping.PopTimeout(&dummy_msg, 1500);
		if (!this->mThreadRunning)
		{
			return;
		}

		Zone2_SOA_UpdateContext();

		while (this->mThreadRunning)
		{
			this->mQueueForThreadStopping.PopTimeout(&dummy_msg, 20000);
			if (this->mThreadRunning)
			{
				Zone2_SOA_UpdateContext();
			}
		}
	}
}

