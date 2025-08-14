#include <Common.h>
#include <SoaDataType.h>
#include <Api_SoaVehicleInfo.hpp>
#include <SoaVehicleInfoSkeletonImpl.hpp>

using namespace eevp::control;

static eevp::control::SoaVehicleInfoSkeletonImpl *s_inst = nullptr;

void SOA_VehicleInfo_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_Wiper/AA/PPort_SOA_VehicleInfo");
		s_inst = new SoaVehicleInfoSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_VehicleInfo_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_VehicleInfo_Update(void)
{
	if (s_inst)
	{
		s_inst->UpdateVehicleInfo();
	}
}
