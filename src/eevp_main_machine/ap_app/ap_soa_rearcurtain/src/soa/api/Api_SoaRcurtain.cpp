#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <VehicleContext.hpp>
#include <RearCurtainManager.hpp>
#include <Api_SoaRcurtain.hpp>
#include <SoaRcurtainSkeletonImpl.hpp>
#if !defined(DISABLE_SOMEIP)
#include <PaconSetting.hpp>
#endif
#include <Log.hpp>
#include <Debug_DataType2String.hpp>

static eevp::control::SoaRcurtainSkeletonImpl *s_inst = nullptr;

void SOA_REARCURTAIN_Init(void)
{
	if (!s_inst)
	{
		ara::core::InstanceSpecifier specifier("SOA_RearCurtain/AA/PPort_SOA_RearCurtain");
		s_inst = new eevp::control::SoaRcurtainSkeletonImpl(specifier);
		s_inst->OfferService();
	}
}

void SOA_REARCURTAIN_Term(void)
{
	if (s_inst)
	{
		delete s_inst;
		s_inst = nullptr;
	}
}

int SOA_REARCURTAIN_MoveMotor(RCtnMotorDir_e dir)
{
	LOG_DEBUG() << "(+) dir=" << getStringEnum_RCtnMotorDir_e(dir) << "\n";
	RearCurtainManager::GetInstance()->MoveMotor(eClientLevel_GuiApps, dir);
	LOG_DEBUG() << "(-)\n";
	return 0;
}

int SOA_REARCURTAIN_MovePosition(int percentage)
{
	LOG_DEBUG() << "(+) percentage=" << percentage << "\n";
	RearCurtainManager::GetInstance()->MovePosition(eClientLevel_GuiApps, percentage);
	LOG_DEBUG() << "(-)\n";
	return 0;
}

void SOA_REARCURTAIN_UpdateStatus()
{
	if (s_inst != nullptr)
	{
		s_inst->UpdateStatus();
	}
}

