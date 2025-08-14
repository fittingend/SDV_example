#include <Common.h>
#include <SoaDataType.h>
#include <Api_SoaWiper.hpp>
#include <Api_Zone4Wiper.hpp>
#include <VehicleContext.hpp>
#include <SoaWiperSkeletonImpl.hpp>

using namespace eevp::control;

static eevp::control::SoaWiperSkeletonImpl *s_inst = nullptr;

void SOA_Wiper_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_Wiper/AA/PPort_SOA_Wiper");
		s_inst = new SoaWiperSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_Wiper_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_Wiper_ControlWiper(WiperOpMode_e mode)
{
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mWiperOpMode != mode)
	{
		context->mWiperOpMode = mode;
		Zone4_Wiper_ControlWiper(mode);
		SOA_Wiper_UpdateWiperStatus();
	}
}

void SOA_Wiper_SetAutoSpeed(bool isAutoSpeed)
{
	VehicleContext *context = VehicleContext::GetInstance();
	if (context->mAutoMode != isAutoSpeed)
	{
		context->mAutoMode = isAutoSpeed;
		SOA_Wiper_UpdateWiperStatus();
	}
}

void SOA_Wiper_UpdateDeviceNormal(void)
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_Wiper_UpdateWiperStatus(void)
{
	if (s_inst)
	{
		s_inst->UpdateWiperStatus();
	}
}