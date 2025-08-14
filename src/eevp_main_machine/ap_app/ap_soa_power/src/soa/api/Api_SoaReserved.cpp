
#include <Api_SoaReserved.hpp>
#include <SoaPowerSkeletonImpl.hpp>

static eevp::control::SoaPowerSkeletonImpl *s_inst = nullptr;

void SOA_ReservedA_Init()
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_Power/AA/PPort_SOA_Power");
		s_inst = new eevp::control::SoaPowerSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_ReservedA_Term()
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_ReservedA_UpdatePowerDeviceNormal()
{
	s_inst->UpdatePowerDeviceNormal();
}

