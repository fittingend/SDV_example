#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <Api_SoaDms.hpp>
#include <Api_Zone5Dms.hpp>
#include <SoaDmsSkeletonImpl.hpp>
#include <DmsManager.hpp>
#include <Log.hpp>

static eevp::control::SoaDmsSkeletonImpl *s_inst = nullptr;

void SOA_DMS_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_DMS/AA/PPort_SOA_DMS");
		s_inst = new eevp::control::SoaDmsSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_DMS_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_DMS_SetPower(ClientLevel_e level, bool on)
{
	DmsManager::GetInstance()->SetPower(level, on);
}

void SOA_DMS_UpdateDeviceNormal()
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_DMS_UpdateDmsStatus()
{
	if (s_inst)
	{
		s_inst->UpdateDmsStatus();
	}
}

void SOA_DMS_UpdateDriverStatus()
{
	if (s_inst)
	{
		s_inst->UpdateDriverStatus();
	}
}

void SOA_DMS_UpdateSwVersion()
{
	if (s_inst)
	{
		s_inst->UpdateSwVersion();
	}
}

void SOA_DMS_NotifyOnDriverNotFocusing()
{
	if (s_inst)
	{
		s_inst->NotifyOnDriverNotFocusing();
	}
}

void SOA_DMS_NotifyOnDriverNotWaken()
{
	if (s_inst)
	{
		s_inst->NotifyOnDriverNotWaken();
	}
}

void SOA_DMS_NotifyOnDriverNotWatchingFront()
{
	if (s_inst)
	{
		s_inst->NotifyOnDriverNotWatchingFront();
	}
}

