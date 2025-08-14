#include <Common.h>
#include <SoaDataType.h>
#include <Zone2RoaProxyImpl.hpp>
#include <Zone2RoaConverter.hpp>
#include <Api_Zone2Roa.hpp>
#include <RoaManager.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace zone2::roa::control;

#define ROA_CONTROL_INIT		1
#define ROA_CONTROL_START	   2
#define ROA_CONTROL_STOP		3
#define ROA_CONTROL_START_DELAY 4

class RoaListener : public IZone2RoaListener
{
public:
	void notifySnsrStatus(const EcmRoaSnsrStatus &fieldValue) override
	{
		LOG_DEBUG() << "EcmRoaSnsrStatus.detInfo = " << (int)fieldValue.detInfo << "\n";
		LOG_DEBUG() << "EcmRoaSnsrStatus.snsrStat = " << (int)fieldValue.snsrStat << "\n";
		LOG_DEBUG() << "EcmRoaSnsrStatus.snsrErrState = " << (int)fieldValue.snsrErrState << "\n";
		LOG_DEBUG() << "EcmRoaSnsrStatus.isNormal = " << (int)fieldValue.isNormal << "\n";

		RoaManager::GetInstance()->DetectCountChanged(Convert_DetectInfo2Count_Zone2SOA(fieldValue.detInfo));
		RoaManager::GetInstance()->SensorErrorChanged(Convert_SensorError_Zone2SOA(fieldValue.snsrErrState));
		RoaManager::GetInstance()->DeviceNormalChanged(Convert_DeviceNormal_Zone2SOA(fieldValue.isNormal));
	}

	void getSnsrStatus() override
	{
	}
};

void Zone2_SOA_Init()
{
	auto *inst = Zone2RoaProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<RoaListener>());
	if (inst->init())
	{
#if 0
		Zone2_SOA_UpdateContext();
#endif
	}
}

void Zone2_SOA_Term()
{
	Zone2RoaProxyImpl::DeleteInstance();
}

void Zone2_SOA_ResetSensor()
{
	LOG_DEBUG() << "(+)\n";
	Zone2RoaProxyImpl::GetInstance()->controlRoa(ROA_CONTROL_INIT);
	LOG_DEBUG() << "(-)\n";
}

void Zone2_SOA_StartSensor()
{
	LOG_DEBUG() << "(+)\n";
	Zone2RoaProxyImpl::GetInstance()->controlRoa(ROA_CONTROL_START);
	LOG_DEBUG() << "(-)\n";
}

void Zone2_SOA_StopSensor()
{
	LOG_DEBUG() << "(+)\n";
	Zone2RoaProxyImpl::GetInstance()->controlRoa(ROA_CONTROL_STOP);
	LOG_DEBUG() << "(-)\n";
}

void Zone2_SOA_UpdateContext()
{
	auto *inst = Zone2RoaProxyImpl::GetInstance();
	EcmRoaSnsrStatus status;
	if (inst->getterSnsrStatus(status))
	{
		RoaManager::GetInstance()->DetectCountChanged(Convert_DetectInfo2Count_Zone2SOA(status.detInfo));
		RoaManager::GetInstance()->SensorErrorChanged(Convert_SensorError_Zone2SOA(status.snsrErrState));
		RoaManager::GetInstance()->DeviceNormalChanged(Convert_DeviceNormal_Zone2SOA(status.isNormal));
	}
}