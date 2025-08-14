#include <Api_Zone5Hvac.hpp>
#include <IZone5HvacListener.hpp>
#include <Zone5HvacProxyImpl.hpp>
#include <HvacManager.hpp>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>
#include <VehicleContext.hpp>

using namespace zone5::hvac::control;

class HvacListener : public IZone5HvacListener {
public:
	void notifyDeviceNormal(const DeviceNormal_e &isNormal) override
	{
		LOG_DEBUG() << "isNormal=" << getStringEnum_DeviceNormal_e(isNormal) << "\n";
		HvacManager::GetInstance()->UpdateDeviceNormal(isNormal);
	}

	void notifyHvacSetting(const HvacStatus_t &setting) override
	{
		LOG_DEBUG() << "setting:" << getString_HvacStatus_t(setting) << "\n";
		HvacManager::GetInstance()->UpdateHvacSetting(setting);
	}

	void notifyHvacStatus(const HvacStatus_t &status) override
	{
		LOG_DEBUG() << "status:" << getString_HvacStatus_t(status) << "\n";
		HvacManager::GetInstance()->UpdateHvacStatus(status);
	}

	void notifyHvacTemps(const HvacTemps_t &temps) override
	{
		LOG_DEBUG() << "temps:" << getString_HvacTemps_t(temps) << "\n";
		HvacManager::GetInstance()->UpdateHvacTemps(temps);
	}
};

void Zone5_HVAC_Init(void)
{
	auto *inst = Zone5HvacProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<HvacListener>());
	inst->init();
}

void Zone5_HVAC_Term()
{
	Zone5HvacProxyImpl::DeleteInstance();
}

void Zone5_HVAC_UpdateContext(void)
{
	auto *inst = Zone5HvacProxyImpl::GetInstance();

	// isDeviceNormal
	DeviceNormal_e isNormal;
	if (inst->getterDeviceNormal(isNormal))
	{
		HvacManager::GetInstance()->UpdateDeviceNormal(isNormal);
	}

	// Setting
	HvacStatus_t setting;
	if (inst->getterHvacSetting(setting))
	{
		HvacManager::GetInstance()->UpdateHvacSetting(setting);
	}

	// Status
	HvacStatus_t status;
	if (inst->getterHvacStatus(status))
	{
		HvacManager::GetInstance()->UpdateHvacStatus(status);
	}

	// Temps
	HvacTemps_t temp;
	if (inst->getterHvacTemps(temp))
	{
		HvacManager::GetInstance()->UpdateHvacTemps(temp);
	}
}

void Zone5_HVAC_SetHvacPower(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetHvacPower(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetAcOnOff(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetAcOnOff(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetAutoTemp(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetAutoTemp(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetSync(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetSync(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetDriverTemp(int temperature)
{
	LOG_DEBUG() << "(+) temperature=" << temperature << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetDriverTemp(temperature);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetPassengerTemp(int temperature)
{
	LOG_DEBUG() << "(+) temperature=" << temperature << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetPassengerTemp(temperature);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetBlowingForce(int force)
{
	LOG_DEBUG() << "(+) force=" << force << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetBlowingForce(force);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetBlowingDirection(HvacBlowDir_e dir)
{
	LOG_DEBUG() << "(+) dir=" << getStringEnum_HvacBlowDir_e(dir) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetBlowingDirection(dir);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetAirSource(HvacAirSource_e src)
{
	LOG_DEBUG() << "(+) src=" << getStringEnum_HvacAirSource_e(src) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetAirSource(src);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetForwardDefrostOn(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetForwardDefrostOn(on);
	LOG_DEBUG() << "(-)\n";
}

void Zone5_HVAC_SetBackwardDefrostOn(bool on)
{
	LOG_DEBUG() << "(+) on=" << getStringEnum_bool(on) << "\n";
	Zone5HvacProxyImpl::GetInstance()->SetBackwardDefrostOn(on);
	LOG_DEBUG() << "(-)\n";
}
