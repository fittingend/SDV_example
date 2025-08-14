#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <VehicleContext.hpp>
#include <MoodLampManager.hpp>
#include <Api_SoaMoodLamp.hpp>
#include <SoaMlmSkeletonImpl.hpp>
#include <Log.hpp>

static eevp::control::SoaMlmSkeletonImpl *s_inst = nullptr;

void SOA_MoodLamp_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_MoodLamp/AA/PPort_SOA_MoodLamp");
		s_inst = new eevp::control::SoaMlmSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_MoodLamp_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_MoodLamp_SetColorIndex(int index)
{
	MoodLampManager::GetInstance()->SetColorIndex(eClientLevel_GuiApps, index);
}

void SOA_MoodLamp_SetMode(MlmMoodMode_e mode)
{
	MoodLampManager::GetInstance()->SetMode(eClientLevel_GuiApps, mode);
}

void SOA_MoodLamp_SetBrightness(int brightness)
{
	MoodLampManager::GetInstance()->SetBrightness(eClientLevel_GuiApps, brightness);
}

void SOA_MoodLamp_UpdateStatus()
{
	if (s_inst)
	{
		s_inst->UpdateStatus();
	}
}
