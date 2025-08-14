#include <Common.h>
#include <SoaDataType.h>
#include <SoaRoaSkeletonImpl.hpp>
#include <Api_SoaRoa.hpp>
#include <RoaManager.hpp>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

static eevp::control::SoaRoaSkeletonImpl *s_inst = nullptr;

void SOA_ROA_Init()
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_ROA/AA/PPort_SOA_ROA");
		s_inst = new eevp::control::SoaRoaSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_ROA_Term()
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

void SOA_ROA_SetMode(RoaMode_e mode)
{
	LOG_DEBUG() << "(+) mode=" << getStringEnum_RoaMode_e(mode) << "\n";
	RoaManager::GetInstance()->SetMode(eClientLevel_GuiApps, mode);
	LOG_DEBUG() << "(-)\n";
}

void SOA_ROA_Reset()
{
	LOG_DEBUG() << "(+)\n";
	RoaManager::GetInstance()->Reset(eClientLevel_GuiApps);
	LOG_DEBUG() << "(-)\n";
}

void SOA_ROA_Start()
{
	LOG_DEBUG() << "(+)\n";
	RoaManager::GetInstance()->Start(eClientLevel_GuiApps);
	LOG_DEBUG() << "(-)\n";
}

void SOA_ROA_Stop()
{
	LOG_DEBUG() << "(+)\n";
	RoaManager::GetInstance()->Stop(eClientLevel_GuiApps);
	LOG_DEBUG() << "(-)\n";
}

void SOA_ROA_UpdateDeviceNormal()
{
	if (s_inst)
	{
		s_inst->UpdateDeviceNormal();
	}
}

void SOA_ROA_UpdateRunningState()
{
	if (s_inst)
	{
		s_inst->UpdateRunningState();
	}
}

void SOA_ROA_UpdateRoaMode()
{
	if (s_inst)
	{
		s_inst->UpdateRoaMode();
	}
}

void SOA_ROA_UpdateDetectState()
{
	if (s_inst)
	{
		s_inst->UpdateDetectState();
	}
}

void SOA_ROA_UpdateDetectCount()
{
	if (s_inst)
	{
		s_inst->UpdateDetectCount();
	}
}

void SOA_ROA_UpdateSensorError()
{
	if (s_inst)
	{
		s_inst->UpdateSensorError();
	}
}