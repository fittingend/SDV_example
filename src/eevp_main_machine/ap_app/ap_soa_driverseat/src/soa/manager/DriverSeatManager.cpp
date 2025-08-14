#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <DriverSeatManager.hpp>
#include <Api_SoaDriverSeat.hpp>
#include <Api_Zone1DrvSeat.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

DriverSeatManager *DriverSeatManager::mInst = nullptr;

DriverSeatManager *DriverSeatManager::GetInstance()
{
	if (DriverSeatManager::mInst == nullptr)
	{
		DriverSeatManager::mInst = new DriverSeatManager();
	}

	return DriverSeatManager::mInst;
}

void DriverSeatManager::RemoveInstance()
{
	if (DriverSeatManager::mInst != nullptr)
	{
		DriverSeatManager::mInst->Stop();
		delete DriverSeatManager::mInst;
		DriverSeatManager::mInst = nullptr;
	}
}

DriverSeatManager::DriverSeatManager()
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

DriverSeatManager::~DriverSeatManager()
{

}

void DriverSeatManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void DriverSeatManager::SetHeight(ClientLevel_e clientLevel, DriverSeatDir_e dir)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _clientLevel = (ClientLevel_e)arg1;
		DriverSeatDir_e _dir = (DriverSeatDir_e)arg2;
		LOG_DEBUG() << "(+) _clientLevel = " << getStringEnum_ClientLevel_e(_clientLevel) << ", _dir = " << getStringEnum_DriverSeatDir_e(_dir) << "\n";
		Zone1_Drvseat_SetHeight(_dir);
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)clientLevel, (int)dir);
}

void DriverSeatManager::SetRecline(ClientLevel_e clientLevel, DriverSeatDir_e dir)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _clientLevel = (ClientLevel_e)arg1;
		DriverSeatDir_e _dir = (DriverSeatDir_e)arg2;
		LOG_DEBUG() << "(+) _clientLevel = " << getStringEnum_ClientLevel_e(_clientLevel) << ", _dir = " << getStringEnum_DriverSeatDir_e(_dir) << "\n";
		Zone1_Drvseat_SetRecline(_dir);
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)clientLevel, (int)dir);
}

void DriverSeatManager::SetSlide(ClientLevel_e clientLevel, DriverSeatDir_e dir)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _clientLevel = (ClientLevel_e)arg1;
		DriverSeatDir_e _dir = (DriverSeatDir_e)arg2;
		LOG_DEBUG() << "(+) _clientLevel = " << getStringEnum_ClientLevel_e(_clientLevel) << ", _dir = " << getStringEnum_DriverSeatDir_e(_dir) << "\n";
		Zone1_Drvseat_SetSlide(_dir);
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)clientLevel, (int)dir);
}

void DriverSeatManager::SetTilt(ClientLevel_e clientLevel, DriverSeatDir_e dir)
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _clientLevel = (ClientLevel_e)arg1;
		DriverSeatDir_e _dir = (DriverSeatDir_e)arg2;
		LOG_DEBUG() << "(+) _clientLevel = " << getStringEnum_ClientLevel_e(_clientLevel) << ", _dir = " << getStringEnum_DriverSeatDir_e(_dir) << "\n";
		Zone1_Drvseat_SetTilt(_dir);
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)clientLevel, (int)dir);
}

void DriverSeatManager::UpdateDeviceNormal(DeviceNormal_e isNormal)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		LOG_DEBUG() << "(+)\n";
		VehicleContext *context = VehicleContext::GetInstance();
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		if (context->mDeviceNormal != _isNormal)
		{
			context->mDeviceNormal = _isNormal;
			SOA_Drvseat_UpdateDeviceNormal();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)isNormal);
	// LOG_DEBUG() << "(-)\n";
}

void DriverSeatManager::UpdateMotorDirection(DriverSeatDir_e height, DriverSeatDir_e recline, DriverSeatDir_e slide, DriverSeatDir_e tilt)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		LOG_DEBUG() << "(+)\n";
		VehicleContext *context = VehicleContext::GetInstance();
		DriverSeatDir_e _height = (DriverSeatDir_e)arg1;
		DriverSeatDir_e _recline = (DriverSeatDir_e)arg2;
		DriverSeatDir_e _slide = (DriverSeatDir_e)arg3;
		DriverSeatDir_e _tilt = (DriverSeatDir_e)arg4;
		bool isUpdated = false;
		if (context->mDirHeight != _height)
		{
			context->mDirHeight = _height;
			isUpdated = true;
		}
		if (context->mDirRecline != _recline)
		{
			context->mDirRecline = _recline;
			isUpdated = true;
		}
		if (context->mDirSlide != _slide)
		{
			context->mDirSlide = _slide;
			isUpdated = true;
		}
		if (context->mDirTilt != _tilt)
		{
			context->mDirTilt = _tilt;
			isUpdated = true;
		}
		if (isUpdated)
		{
			SOA_Drvseat_UpdateMotorDirection();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)height, (int)recline, (int)slide, (int)tilt);
	// LOG_DEBUG() << "(-)\n";
}

void DriverSeatManager::UpdateMotorPosition(int height, int recline, int slide, int tilt)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		LOG_DEBUG() << "(+)\n";
		VehicleContext *context = VehicleContext::GetInstance();
		int _height = (int)arg1;
		int _recline = (int)arg2;
		int _slide = (int)arg3;
		int _tilt = (int)arg4;
		bool isUpdated = false;
		if (context->mPosHeight != _height)
		{
			context->mPosHeight = _height;
			isUpdated = true;
		}
		if (context->mPosRecline != _recline)
		{
			context->mPosRecline = _recline;
			isUpdated = true;
		}
		if (context->mPosSlide != _slide)
		{
			context->mPosSlide = _slide;
			isUpdated = true;
		}
		if (context->mPosTilt != _tilt)
		{
			context->mPosTilt = _tilt;
			isUpdated = true;
		}
		if (isUpdated)
		{
			SOA_Drvseat_UpdateMotorPosition();
		}
		LOG_DEBUG() << "(-)\n";
		return 0;
	}, (int)height, (int)recline, (int)slide, (int)tilt);
	// LOG_DEBUG() << "(-)\n";
}