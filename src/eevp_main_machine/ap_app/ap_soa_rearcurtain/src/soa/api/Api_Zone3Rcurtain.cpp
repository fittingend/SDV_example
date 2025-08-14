#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <string>
#include <Api_Zone3Rcurtain.hpp>
#include <RearCurtainManager.hpp>
#include <Zone3RcurtainConverter.hpp>
#include <Zone3RcurtainProxyImpl.hpp>
#include <SimpleTimerItem.hpp>
#include <SimpleTimer.hpp>
#include <VehicleContext.hpp>
#include <iostream>
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace zone3::rcurtain::control;

class RearCurtainListener : public zone3::rcurtain::control::IZone3RearCurtainListener
{
	void notifyRcurtainStatus(const zone3::rcurtain::control::EcuRcurtainStatus &fieldValue)
	{
		LOG_DEBUG() << "EcuRcurtainStatus.curMotorDir = " << (int)fieldValue.curMotorDir << "\n";
		LOG_DEBUG() << "EcuRcurtainStatus.curMotorLimit = " << (int)fieldValue.curMotorLimit << "\n";
		LOG_DEBUG() << "EcuRcurtainStatus.motorCurrent = " << (int)fieldValue.motorCurrent << "\n";
		LOG_DEBUG() << "EcuRcurtainStatus.isNormal = " << (int)fieldValue.isNormal << "\n";

		RearCurtainManager::GetInstance()->EventStatus(Convert_Position_Zone2SOA(fieldValue.curMotorDir), Convert_DeviceNormal_Zone2SOA(fieldValue.isNormal));
	}

	void getRcurtainStatus()
	{
	}
};

void Zone3_RearCurtain_Init(void)
{
	auto *inst = Zone3RearCurtainProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<RearCurtainListener>());
	if (inst->init())
	{

#if 0
		EcuRcurtainStatus status;
		if (inst->getterRcurtainStatus(status))
		{
			update_vehicle_context_by_EcuRcurtainStatus(status);
		}
#endif
	}
}

void Zone3_RearCurtain_Term(void)
{
	Zone3RearCurtainProxyImpl::DeleteInstance();
}

void Zone3_RearCurtain_PowerOn(PowerState_e power)
{

}

void Zone3_RearCurtain_MovePosition(RCtnPos_e pos)
{
	LOG_DEBUG() << "(+) pos=" << getStringEnum_RCtnPos_e(pos) << "\n";
	Zone3RearCurtainProxyImpl::GetInstance()->controlMotor(Convert_Position_SOA2Zone(pos));
	LOG_DEBUG() << "(-)\n";
}

void Zone3_RearCurtain_UpdateContext(void)
{
	auto *inst = Zone3RearCurtainProxyImpl::GetInstance();

	EcuRcurtainStatus status;
	if (inst->getterRcurtainStatus(status))
	{
		RearCurtainManager::GetInstance()->EventStatus(Convert_Position_Zone2SOA(status.curMotorDir), Convert_DeviceNormal_Zone2SOA(status.isNormal));
	}
}
