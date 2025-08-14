#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <Api_SoaDriverSeat.hpp>
#include <DriverSeatManager.hpp>
#include <SoaDriverSeatSkeletonImpl.hpp>
#include <Log.hpp>

static eevp::control::SoaDriverSeatSkeletonImpl *s_inst = nullptr;

void SOA_Drvseat_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_DriverSeat/AA/PPort_SOA_DriverSeat");
		s_inst = new eevp::control::SoaDriverSeatSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_Drvseat_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_Drvseat_SetHeight(DriverSeatDir_e dir)
{
	// LOG_DEBUG() << "(+)\n";
	DriverSeatManager::GetInstance()->SetHeight(eClientLevel_GuiApps, dir);
	// LOG_DEBUG() << "(-)\n";
}

void SOA_Drvseat_SetRecline(DriverSeatDir_e dir)
{
	// LOG_DEBUG() << "(+)\n";
	DriverSeatManager::GetInstance()->SetRecline(eClientLevel_GuiApps, dir);
	// LOG_DEBUG() << "(-)\n";
}

void SOA_Drvseat_SetSlide(DriverSeatDir_e dir)
{
	// LOG_DEBUG() << "(+)\n";
	DriverSeatManager::GetInstance()->SetSlide(eClientLevel_GuiApps, dir);
	// LOG_DEBUG() << "(-)\n";
}

void SOA_Drvseat_SetTilt(DriverSeatDir_e dir)
{
	// LOG_DEBUG() << "(+)\n";
	DriverSeatManager::GetInstance()->SetTilt(eClientLevel_GuiApps, dir);
	// LOG_DEBUG() << "(-)\n";
}

void SOA_Drvseat_UpdateDeviceNormal()
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_Drvseat_UpdateMotorDirection()
{
	if (s_inst)
	{
		s_inst->UpdateMotorDirection();
	}
}

void SOA_Drvseat_UpdateMotorPosition()
{
	if (s_inst)
	{
		s_inst->UpdateMotorPosition();
	}
}