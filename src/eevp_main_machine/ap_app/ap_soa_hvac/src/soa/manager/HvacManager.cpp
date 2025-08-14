#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <HvacManager.hpp>
#include <Api_SoaHvac.hpp>
#include <Api_Zone5Hvac.hpp>
#include <Log.hpp>

HvacManager *HvacManager::mInst = nullptr;

HvacManager *HvacManager::GetInstance()
{
	if (HvacManager::mInst == nullptr)
	{
		HvacManager::mInst = new HvacManager();
	}

	return HvacManager::mInst;
}

void HvacManager::RemoveInstance()
{
	if (HvacManager::mInst != nullptr)
	{
		HvacManager::mInst->Stop();
		delete HvacManager::mInst;
		HvacManager::mInst = nullptr;
	}
}

HvacManager::HvacManager()
{
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		this->init();
		return 0;
	});
}

HvacManager::~HvacManager()
{

}

void HvacManager::init()
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetHvacPower(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetHvacPower(_on);

		if (context->mSetting.mPowerOn != _on)
		{
			context->mSetting.mPowerOn = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetAcOnOff(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetAcOnOff(_on);

		if (context->mSetting.mAcOnOff != _on)
		{
			context->mSetting.mAcOnOff = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetAutoTemp(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetAutoTemp(_on);

		if (context->mSetting.mAutoMode != _on)
		{
			context->mSetting.mAutoMode = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetSync(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetSync(_on);

		if (context->mSetting.mSyncMode != _on)
		{
			context->mSetting.mSyncMode = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetDriverTemp(ClientLevel_e level, int temperature)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _temperature = (int)arg2;

		Zone5_HVAC_SetDriverTemp(_temperature);

		if (context->mSetting.mDriverTemp != _temperature)
		{
			context->mSetting.mDriverTemp = _temperature;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)temperature);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetPassengerTemp(ClientLevel_e level, int temperature)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _temperature = (int)arg2;

		Zone5_HVAC_SetPassengerTemp(_temperature);

		if (context->mSetting.mPassengerTemp != _temperature)
		{
			context->mSetting.mPassengerTemp = _temperature;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)temperature);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetBlowingForce(ClientLevel_e level, int force)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		int _force = (int)arg2;

		Zone5_HVAC_SetBlowingForce(_force);

		if (context->mSetting.mBlowingForce != _force)
		{
			context->mSetting.mBlowingForce = _force;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)force);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetBlowingDirection(ClientLevel_e level, HvacBlowDir_e dir)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		HvacBlowDir_e _dir = (HvacBlowDir_e)arg2;

		Zone5_HVAC_SetBlowingDirection(_dir);

		if (context->mSetting.mBlowDir != _dir)
		{
			context->mSetting.mBlowDir = _dir;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)dir);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetAirSource(ClientLevel_e level, HvacAirSource_e src)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		HvacAirSource_e _src = (HvacAirSource_e)arg2;

		Zone5_HVAC_SetAirSource(_src);

		if (context->mSetting.mAirSrc != _src)
		{
			context->mSetting.mAirSrc = _src;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)src);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetForwardDefrostOn(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetForwardDefrostOn(_on);

		if (context->mSetting.mForwardFrostOn != _on)
		{
			context->mSetting.mForwardFrostOn = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::SetBackwardDefrostOn(ClientLevel_e level, bool on)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		ClientLevel_e _level = (ClientLevel_e)arg1;
		bool _on = (bool)arg2;

		Zone5_HVAC_SetBackwardDefrostOn(_on);

		if (context->mSetting.mBackwardFrostOn != _on)
		{
			context->mSetting.mBackwardFrostOn = _on;
			SOA_HVAC_UpdateHvacSetting();
		}

		return 0;
	}, (int)level, (int)on);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::UpdateDeviceNormal(DeviceNormal_e isNormal)
{
	// LOG_DEBUG() << "(+)\n";
	this->executeFunction([this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		DeviceNormal_e _isNormal = (DeviceNormal_e)arg1;
		context->mDeviceNormal = _isNormal;
		SOA_HVAC_UpdateDeviceNormal();
		return 0;
	}, (int)isNormal);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::UpdateHvacSetting(const HvacStatus_t &setting)
{
	// LOG_DEBUG() << "(+)\n";
	HvacStatus_t *dupSetting = new HvacStatus_t();
	*dupSetting = setting;

	FuncMsgItem_t msgItem;
	msgItem.mFunc = [this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		HvacStatus_t *_setting = (HvacStatus_t *)argv;
		context->mSetting = *_setting;
		delete _setting;
		SOA_HVAC_UpdateHvacSetting();
		return 0;
	};
	msgItem.mArgPtr = (void *)dupSetting;
	this->sendMessage(msgItem);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::UpdateHvacStatus(const HvacStatus_t &status)
{
	// LOG_DEBUG() << "(+)\n";
	HvacStatus_t *dupStatus = new HvacStatus_t();
	*dupStatus = status;

	FuncMsgItem_t msgItem;
	msgItem.mFunc = [this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		HvacStatus_t *_status = (HvacStatus_t *)argv;
		context->mStatus = *_status;
		delete _status;
		SOA_HVAC_UpdateHvacStatus();
		return 0;
	};
	msgItem.mArgPtr = (void *)dupStatus;
	this->sendMessage(msgItem);
	// LOG_DEBUG() << "(-)\n";
}

void HvacManager::UpdateHvacTemps(const HvacTemps_t &temps)
{
	// LOG_DEBUG() << "(+)\n";
	HvacTemps_t *dupTemps = new HvacTemps_t();
	*dupTemps = temps;

	FuncMsgItem_t msgItem;
	msgItem.mFunc = [this](int arg1, int arg2, int arg3, int arg4, void *argv) {
		VehicleContext *context = VehicleContext::GetInstance();
		HvacTemps_t *_temps = (HvacTemps_t *)argv;
		context->mTemps = *_temps;
		delete _temps;
		SOA_HVAC_UpdateHvacTemps();
		return 0;
	};
	msgItem.mArgPtr = (void *)dupTemps;
	this->sendMessage(msgItem);
	// LOG_DEBUG() << "(-)\n";
}
