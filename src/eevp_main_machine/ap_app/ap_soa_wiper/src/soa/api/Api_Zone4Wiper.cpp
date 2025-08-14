#include <Common.h>
#include <SoaDataType.h>
#include <Zone4WiperProxyImpl.hpp>
#include <Zone4WiperConverter.hpp>
#include <Api_Zone4Wiper.hpp>
#include <WiperManager.hpp>
#include <VehicleContext.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

using namespace zone4::wiper::control;

class WiperListener : public IZone4WiperListener {
public:
	void notifyWiperOpStatus(const zone4::wiper::control::EcuWiperOperationStatus &fieldValue) override
	{
		LOG_DEBUG() << "EcuWiperOperationStatus.curOperationMode = " << (int)fieldValue.curOperationMode << "\n";
		LOG_DEBUG() << "EcuWiperOperationStatus.isNormal = " << (int)fieldValue.isNormal << "\n";
		WiperManager::GetInstance()->MotorOpChanged(Convert_WiperMode_Zone2SOA(fieldValue.curOperationMode));
		WiperManager::GetInstance()->DeviceNormalChanged(Convert_DeviceNormal_Zone2SOA(fieldValue.isNormal));
	}

	virtual void getWiperOpStatus()
	{
	}
};

void Zone4_Wiper_Init(void)
{
	auto *inst = Zone4WiperProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<WiperListener>());
	if (inst->init())
	{


#if 0
		EcuWiperOperationStatus status;
		if (inst->getterWiperOpStatus(status))
		{
			updateContextByWiperOpStatus(status);
		}
#endif
	}
}

void Zone4_Wiper_Term(void)
{
	Zone4WiperProxyImpl::DeleteInstance();
}

void Zone4_Wiper_ControlWiper(WiperOpMode_e opMode)
{
	LOG_DEBUG() << "(+) opMode=" << getStringEnum_WiperOpMode_e(opMode) << "\n";
	Zone4WiperProxyImpl::GetInstance()->controlWiper(Convert_WiperMode_SOA2Zone(opMode));
	LOG_DEBUG() << "(-)\n";
}

void Zone4_Wiper_UpdateContext(void)
{
	auto *inst = Zone4WiperProxyImpl::GetInstance();
	EcuWiperOperationStatus status;
	if (inst->getterWiperOpStatus(status))
	{
		WiperManager::GetInstance()->MotorOpChanged(Convert_WiperMode_Zone2SOA(status.curOperationMode));
		WiperManager::GetInstance()->DeviceNormalChanged(Convert_DeviceNormal_Zone2SOA(status.isNormal));
	}
}