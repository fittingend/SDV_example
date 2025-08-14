#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <string>
#include <Api_Zone1MoodLamp.hpp>
#include <MoodLampManager.hpp>
#include <Zone1MlmProxyImpl.hpp>
#include <VehicleContext.hpp>
#include <iostream>
#include <Log.hpp>

using namespace zone1::mlm::control;

static void zone1_convertAndUpdateStatus(EcuMlmState status)
{
	DeviceNormal_e isNormal;

	switch (status.isNormal)
	{
	case 0:
		isNormal = eDeviceNormal_Ok;
		break;
	case 1:
		isNormal = eDeviceNormal_Abnormal;
		break;
	default:
		isNormal = eDeviceNormal_Abnormal;
		break;
	}

	MoodLampManager::GetInstance()->UpdateStatus((int)status.colorIndex, (int)status.brightness, isNormal);
}

class MoodLampListener : public zone1::mlm::control::IZone1MlmListener
{
	void notifyMlmStatus(const zone1::mlm::control::EcuMlmState &fieldValue) override
	{
		LOG_DEBUG() << "EcuMlmState.colorIndex = " << (int)fieldValue.colorIndex << "\n";
		LOG_DEBUG() << "EcuMlmState.brightness = " << (int)fieldValue.brightness << "\n";
		LOG_DEBUG() << "EcuMlmState.isNormal = " << (int)fieldValue.isNormal << "\n";
		zone1_convertAndUpdateStatus(fieldValue);
	}

	void getMlmStatus() override
	{
	}
};

void Zone1_MoodLamp_Init(void)
{
	auto *inst = Zone1MlmProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<MoodLampListener>());
	inst->init();
}

void Zone1_MoodLamp_Term(void)
{
	Zone1MlmProxyImpl::DeleteInstance();
}


void Zone1_MoodLamp_PowerOn(PowerState_e power)
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void Zone1_MoodLamp_SetColorIndex(int colorIndex)
{
	LOG_DEBUG() << "(+)\n";
	Zone1MlmProxyImpl *inst = Zone1MlmProxyImpl::GetInstance();
	if (inst)
	{
		inst->setMlmColor((std::uint8_t)colorIndex);
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_MoodLamp_SetBrightness(int brightness)
{
	LOG_DEBUG() << "(+)\n";
	Zone1MlmProxyImpl *inst = Zone1MlmProxyImpl::GetInstance();
	if (inst)
	{
		inst->setMlmBrightness((std::uint8_t)brightness);
	}
	LOG_DEBUG() << "(-)\n";
}

void Zone1_MoodLamp_SetColorMode(MlmMoodMode_e eMoodMode)
{
	LOG_DEBUG() << "(+)\n";

	LOG_DEBUG() << "(-)\n";
}

void Zone1_MoodLamp_UpdateStatus(void)
{
	LOG_DEBUG() << "(+)\n";
	Zone1MlmProxyImpl *inst = Zone1MlmProxyImpl::GetInstance();
	if (inst)
	{
		EcuMlmState status;
		if (inst->getterMlmStatus(status))
		{
			zone1_convertAndUpdateStatus(status);
		}
	}
	LOG_DEBUG() << "(-)\n";
}
