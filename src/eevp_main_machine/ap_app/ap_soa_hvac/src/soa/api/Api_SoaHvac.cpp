#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <Api_SoaHvac.hpp>
#include <SoaHvacSkeletonImpl.hpp>
#include <HvacManager.hpp>
#include <Log.hpp>

static eevp::control::SoaHvacSkeletonImpl *s_inst = nullptr;

void SOA_HVAC_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_HVAC/AA/PPort_SOA_HVAC");
		s_inst = new eevp::control::SoaHvacSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_HVAC_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_HVAC_SetHvacPower(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetHvacPower(level, on);
}

void SOA_HVAC_SetAcOnOff(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetAcOnOff(level, on);
}

void SOA_HVAC_SetAutoTemp(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetAutoTemp(level, on);
}

void SOA_HVAC_SetSync(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetSync(level, on);
}

void SOA_HVAC_SetDriverTemp(ClientLevel_e level, int temperature)
{
	HvacManager::GetInstance()->SetDriverTemp(level, temperature);
}

void SOA_HVAC_SetPassengerTemp(ClientLevel_e level, int temperature)
{
	HvacManager::GetInstance()->SetPassengerTemp(level, temperature);
}

void SOA_HVAC_SetBlowingForce(ClientLevel_e level, int force)
{
	HvacManager::GetInstance()->SetBlowingForce(level, force);
}

void SOA_HVAC_SetBlowingDirection(ClientLevel_e level, HvacBlowDir_e dir)
{
	HvacManager::GetInstance()->SetBlowingDirection(level, dir);
}

void SOA_HVAC_SetAirSource(ClientLevel_e level, HvacAirSource_e src)
{
	HvacManager::GetInstance()->SetAirSource(level, src);
}

void SOA_HVAC_SetForwardDefrostOn(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetForwardDefrostOn(level, on);
}

void SOA_HVAC_SetBackwardDefrostOn(ClientLevel_e level, bool on)
{
	HvacManager::GetInstance()->SetBackwardDefrostOn(level, on);
}

void SOA_HVAC_UpdateDeviceNormal()
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_HVAC_UpdateHvacSetting()
{
	if (s_inst)
	{
		s_inst->UpdateHvacSetting();
	}
}

void SOA_HVAC_UpdateHvacStatus()
{
	if (s_inst)
	{
		s_inst->UpdateHvacStatus();
	}
}

void SOA_HVAC_UpdateHvacTemps()
{
	if (s_inst)
	{
		s_inst->UpdateHvacTemps();
	}
}

void SOA_HVAC_UpdateSwVersion()
{
	if (s_inst)
	{
		s_inst->UpdateSwVersion();
	}
}