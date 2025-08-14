#include <Common.h>
#include <SoaDataType.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <WiperManager.hpp>
#include <Api_SoaVehicleInfo.hpp>
#include <Api_SoaWiper.hpp>
#include <Api_Zone4Wiper.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

WiperManager *WiperManager::mInst = nullptr;

WiperManager *WiperManager::GetInstance()
{
	if (WiperManager::mInst == nullptr)
	{
		WiperManager::mInst = new WiperManager();
	}

	return WiperManager::mInst;
}

void WiperManager::RemoveInstance()
{
	if (WiperManager::mInst != nullptr)
	{
		WiperManager::mInst->Stop();
		delete WiperManager::mInst;
		WiperManager::mInst = nullptr;
	}
}

WiperManager::WiperManager()
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

WiperManager::~WiperManager()
{

}

void WiperManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::cmdSetMotor(ClientLevel_e level, WiperOpMode_e opmode)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << ", opMode=" << getStringEnum_WiperOpMode_e(opmode) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mWiperOpMode != opmode)
	{
		context->mWiperOpMode = opmode;
		Zone4_Wiper_ControlWiper(opmode);
		SOA_Wiper_UpdateWiperStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::cmdSetAuto(ClientLevel_e level, bool isAuto)
{
	LOG_DEBUG() << "(+) level=" << getStringEnum_ClientLevel_e(level) << ", isAuto=" << getStringEnum_bool(isAuto) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mAutoMode != isAuto)
	{
		context->mAutoMode = isAuto;
		SOA_Wiper_UpdateWiperStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::onDeviceNormalChanged(DeviceNormal_e isNormal)
{
	LOG_DEBUG() << "(+) isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mDeviceNormal != isNormal)
	{
		context->mDeviceNormal = isNormal;
		SOA_Wiper_UpdateDeviceNormal();
	}

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::onAutoModeChanged(bool isAuto)
{
	LOG_DEBUG() << "(+) isAuto=" << getStringEnum_bool(isAuto) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mAutoMode != isAuto)
	{
		context->mAutoMode = isAuto;
		SOA_Wiper_UpdateWiperStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::onMotorOpChanged(WiperOpMode_e opmode)
{
	LOG_DEBUG() << "(+) opMode=" << getStringEnum_WiperOpMode_e(opmode) << "\n";

	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mWiperOpMode != opmode)
	{
		context->mWiperOpMode = opmode;
		SOA_Wiper_UpdateWiperStatus();
	}

	LOG_DEBUG() << "(-)\n";
}

void WiperManager::onVehicleInfoReceived(int speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2)
{
	VehicleContext *context = VehicleContext::GetInstance();
	context->mVehicleSpeed = speed;
	context->mGear = gear;
	context->mAcc = acc;
	context->mIgn1 = ign1;
	context->mIgn2 = ign2;
	SOA_VehicleInfo_Update();
}

void WiperManager::onMfswReceived(MfswWiperSw_e wiper, MfswWasherSw_e washer, MfswWiperIntStepSw_e int_step, MfswWiperMistSw_e mist)
{
	
}

void WiperManager::SetMotor(ClientLevel_e level, WiperOpMode_e opmode)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		WiperOpMode_e _opmode = (WiperOpMode_e)arg2;
		this->cmdSetMotor(_level, _opmode);
		return 0;
	}, (int)level, (int)opmode);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::SetAuto(ClientLevel_e level, bool isAuto)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _isAuto = (bool)arg2;
		this->cmdSetAuto(_level, _isAuto);
		return 0;
	}, (int)level, (int)isAuto);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::DeviceNormalChanged(DeviceNormal_e isNormal)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		this->onDeviceNormalChanged(_isNormal);
		return 0;
	}, (int)isNormal);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::AutoModeChanged(bool isAuto)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		bool _isAuto = (bool)arg1;
		this->onAutoModeChanged(_isAuto);
		return 0;
	}, (int)isAuto);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::MotorOpChanged(WiperOpMode_e opmode)
{
	LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		WiperOpMode_e _opmode = (WiperOpMode_e)arg1;
		this->onMotorOpChanged(_opmode);
		return 0;
	}, (int)opmode);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::VehicleInfoReceived(int speed, GearState_e gear, AccState_e acc, IgnState_e ign1, IgnState_e ign2)
{
	LOG_DEBUG() << "(+)\n";
	int _arg1 = speed;
	int _arg2 = (((int)gear << 8) & 0xFF) | ((int)acc & 0xFF);
	int _arg3 = (((int)ign1 << 8) & 0xFF) | ((int)ign2 & 0xFF);
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		int _speed = arg1;
		GearState_e _gear = (GearState_e)((arg2 >> 8) & 0xFF);
		AccState_e _acc = (AccState_e)(arg2 & 0xFF);
		IgnState_e _ign1 = (IgnState_e)((arg3 >> 8) & 0xFF);
		IgnState_e _ign2 = (IgnState_e)(arg3 & 0xFF);		

		this->onVehicleInfoReceived(_speed, _gear, _acc, _ign1, _ign2);
		return 0;
	}, _arg1, _arg2, _arg3);
	LOG_DEBUG() << "(-)\n";
}

void WiperManager::MfswReceived(MfswWiperSw_e wiper, MfswWasherSw_e washer, MfswWiperIntStepSw_e int_step, MfswWiperMistSw_e mist)
{
	LOG_DEBUG() << "(+)\n";
	int _arg1 = (((int)wiper << 8) & 0xFF) | ((int)washer & 0xFF);;
	int _arg2 = (((int)int_step << 8) & 0xFF) | ((int)mist & 0xFF);
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		MfswWiperSw_e _wiper = (MfswWiperSw_e)((arg1 >> 8) & 0xFF);
		MfswWasherSw_e _washer = (MfswWasherSw_e)(arg1 & 0xFF);
		MfswWiperIntStepSw_e _int_step = (MfswWiperIntStepSw_e)((arg2 >> 8) & 0xFF);
		MfswWiperMistSw_e _mist = (MfswWiperMistSw_e)(arg2 & 0xFF);		

		this->onMfswReceived(_wiper, _washer, _int_step, _mist);
		return 0;
	}, _arg1, _arg2);
	LOG_DEBUG() << "(-)\n";
}
