#include <Api_SoaDms.hpp>
#include <Api_Zone5Dms.hpp>
#include <IZone5DmsListener.hpp>
#include <Zone5DmsProxyImpl.hpp>
#include <Zone5DmsConverter.hpp>
#include <DmsManager.hpp>
#include <Log.hpp>
#include <VehicleContext.hpp>

using namespace zone5::dms::control;

class DmsListener : public IZone5DmsListener {
public:
	void notifyDeviceNormal(const DeviceNormal_e &isNormal) override
	{
		DmsManager::GetInstance()->DeviceNormalChanged(isNormal);
	}

	void notifyDmsStatus(const bool &powerOn, const DmsDriverInside_e &inside) override
	{
		DmsManager::GetInstance()->DmsStatusChanged(powerOn, inside);
	}

	void notifyDriverStatus(const DmsFatigue_e &fatigue, const DmsGazingDir_e &dir, const DmsOnPhone_e &phone, const DmsSmoking_e &smoking) override
	{
		DmsManager::GetInstance()->DriverStatusChanged(dir, fatigue, smoking, phone);
	}
};

void Zone5_DMS_Init(void)
{
	LOG_DEBUG() << "(+) \n";
	auto *inst = Zone5DmsProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<DmsListener>());
	inst->init();
	LOG_DEBUG() << "(-) \n";
}

void Zone5_DMS_Term()
{
	Zone5DmsProxyImpl::DeleteInstance();
}

void Zone5_DMS_UpdateContext(void)
{
	auto *inst = Zone5DmsProxyImpl::GetInstance();

	// isDeviceNormal
	{
		DeviceNormal_e isNormal;
		if (inst->getterDeviceNormal(isNormal))
		{
			DmsManager::GetInstance()->DeviceNormalChanged(isNormal);
		}
	}

	// DmsStatus
	{
		bool powerOn;
		DmsDriverInside_e inside;
		if (inst->getterDmsStatus(powerOn, inside))
		{
			DmsManager::GetInstance()->DmsStatusChanged(powerOn, inside);
		}
	}

	// DriverStatus
	{
		DmsFatigue_e fatigue;
		DmsGazingDir_e dir;
		DmsOnPhone_e phone;
		DmsSmoking_e smoking;
		DmsDriverInside_e inside;
		if (inst->getterDmsDriverStatus(fatigue, dir, phone, smoking))
		{
			DmsManager::GetInstance()->DriverStatusChanged(dir, fatigue, smoking, phone);
		}
	}
}

void Zone5_DMS_SetPower(bool power)
{
	LOG_DEBUG() << "(+) power = " << ((power) ? "true" : "false") << "\n";
	Zone5DmsProxyImpl::GetInstance()->SetPower(power);
	LOG_DEBUG() << "(-)\n";
}

