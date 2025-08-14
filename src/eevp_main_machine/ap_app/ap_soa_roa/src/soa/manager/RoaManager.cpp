#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <RoaManager.hpp>
#include <Api_SoaRoa.hpp>
#include <Api_Zone2Roa.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

RoaManager *RoaManager::mInst = nullptr;

RoaManager *RoaManager::GetInstance()
{
	if (RoaManager::mInst == nullptr)
	{
		RoaManager::mInst = new RoaManager();
	}

	return RoaManager::mInst;
}

void RoaManager::RemoveInstance()
{
	if (RoaManager::mInst != nullptr)
	{
		delete RoaManager::mInst;
		RoaManager::mInst = nullptr;
	}
}

RoaManager::RoaManager()
{
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

RoaManager::~RoaManager()
{

}

void RoaManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::cmdSetMode(ClientLevel_e level, RoaMode_e mode)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << ", mode=" << getStringEnum_RoaMode_e(mode) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mRoaMode != mode)
	{
		context->mRoaMode = mode;
		// ToDo:

		SOA_ROA_UpdateRoaMode();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::cmdReset(ClientLevel_e level)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	Zone2_SOA_ResetSensor();
	if (context->mDetectCount != 0)
	{
		context->mDetectCount = 0;
		SOA_ROA_UpdateDetectState();
		SOA_ROA_UpdateDetectCount();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::cmdStart(ClientLevel_e level)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mRunningState != eRoaRunningState_On)
	{
		Zone2_SOA_StartSensor();
		context->mRunningState = eRoaRunningState_On;
		SOA_ROA_UpdateRunningState();
	}
	else
	{
		context->mDetectCount = 0;
		Zone2_SOA_ResetSensor();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::cmdStop(ClientLevel_e level)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mRunningState == eRoaRunningState_On)
	{
		Zone2_SOA_StopSensor();
		context->mRunningState = eRoaRunningState_Off;
		SOA_ROA_UpdateRunningState();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::onDeviceNormalChanged(DeviceNormal_e isNormal)
{
	LOG_DEBUG() << "(+) isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mIsNormal != isNormal)
	{
		context->mIsNormal = isNormal;
		SOA_ROA_UpdateDeviceNormal();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::onRunningStateChanged(RoaRunningState_e runningState)
{
	LOG_DEBUG() << "(+) runningState=" << getStringEnum_RoaRunningState_e(runningState) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mRunningState != runningState)
	{
		context->mRunningState = runningState;
		SOA_ROA_UpdateRunningState();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::onRoaModeChanged(RoaMode_e mode)
{
	LOG_DEBUG() << "(+) mode=" << getStringEnum_RoaMode_e(mode) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mRoaMode != mode)
	{
		context->mRoaMode = mode;
		SOA_ROA_UpdateRoaMode();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::onDetectCountChanged(int count)
{
	LOG_DEBUG() << "(+) count=" << count << "\n";

	VehicleContext *context = VehicleContext::GetInstance();

	if (context->mDetectCount != count)
	{
		context->mDetectCount = count;
		SOA_ROA_UpdateDetectCount();
	}

	int srcCntState = (context->mDetectCount >= 2) ? 2 : ((context->mDetectCount <= 0) ? 0 : 1);
	int dstCntState = (count >= 2) ? 2 : ((count <= 0) ? 0 : 1);
	if (srcCntState != dstCntState)
	{
		SOA_ROA_UpdateDetectState();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::onSensorErrorChanged(RoaSensorError_e err)
{
	LOG_DEBUG() << "(+) sensorError=" << getStringEnum_RoaSensorError_e(err) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mSensorError != err)
	{
		context->mSensorError = err;
		SOA_ROA_UpdateSensorError();
	}

	LOG_DEBUG() << "(-)\n";
}

void RoaManager::SetMode(ClientLevel_e level, RoaMode_e mode)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		RoaMode_e _mode = (RoaMode_e)arg2;
		this->cmdSetMode(_level, _mode);
		return 0;
	}, (int)level, (int)mode);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::Reset(ClientLevel_e level)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		this->cmdReset(_level);
		return 0;
	}, (int)level);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::Start(ClientLevel_e level)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		this->cmdStart(_level);
		return 0;
	}, (int)level);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::Stop(ClientLevel_e level)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		this->cmdStop(_level);
		return 0;
	}, (int)level);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::DeviceNormalChanged(DeviceNormal_e isNormal)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		this->onDeviceNormalChanged(_isNormal);
		return 0;
	}, (int)isNormal);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::RunningStateChanged(RoaRunningState_e runningState)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		RoaRunningState_e _runningState = (RoaRunningState_e)arg1;
		this->onRunningStateChanged(_runningState);
		return 0;
	}, (int)runningState);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::RoaModeChanged(RoaMode_e mode)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		RoaMode_e _mode = (RoaMode_e)arg1;
		this->onRoaModeChanged(_mode);
		return 0;
	}, (int)mode);
	LOG_DEBUG() << "(-)\n";
}

void RoaManager::DetectCountChanged(int count)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		int _count = (int)arg1;
		this->onDetectCountChanged(_count);
		return 0;
	}, (int)count);
	LOG_DEBUG() << "(-)\n";

}

void RoaManager::SensorErrorChanged(RoaSensorError_e err)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([&](int arg1, int arg2, int arg3, int arg4, void *argv) {
		RoaSensorError_e _err = (RoaSensorError_e)arg1;
		this->onSensorErrorChanged(_err);
		return 0;
	}, (int)err);
	LOG_DEBUG() << "(-)\n";
}

