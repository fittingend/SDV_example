#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <Api_SoaSmartFilm.hpp>
#include <SoaSmartFilmSkeletonImpl.hpp>
#include <SmartFilmManager.hpp>
#include <Log.hpp>

static eevp::control::SoaSmartFilmSkeletonImpl *s_inst = nullptr;

void SOA_SMARTFILM_Init()
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_SmartFilm/AA/PPort_SOA_SmartFilm");
		s_inst = new eevp::control::SoaSmartFilmSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_SMARTFILM_Term()
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_SMARTFILM_SetOpacity(ClientLevel_e level, SmtfilmPos_e pos, int opacity)
{
	SmartFilmManager::GetInstance()->SetOpacity(level, pos, opacity);
}

void SOA_SMARTFILM_SetAllOpacity(ClientLevel_e level, int opacity)
{
	SmartFilmManager::GetInstance()->SetAllOpacity(level, opacity);
}

void SOA_SMARTFILM_SetAuto(ClientLevel_e level, bool on)
{
	SmartFilmManager::GetInstance()->SetAuto(level, on);
}

void SOA_SMARTFILM_UpdateDeviceNormal()
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_SMARTFILM_UpdateSmtfilmAuto()
{
	if (s_inst)
	{
		s_inst->UpdateSmtfilmAuto();
	}
}

void SOA_SMARTFILM_UpdateSmtfilmOpacities()
{
	if (s_inst)
	{
		s_inst->UpdateSmtfilmOpacities();
	}
}

