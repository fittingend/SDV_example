#include <Common.h>
#include <SoaDataType.h>
#include <SoaVehicleInfoConverter.hpp>

// GearState_e
eevp::control::SoaGearState Convert_GearState_SOA2API(GearState_e gear)
{
	switch (gear)
	{
	case eGearState_P:
		return eevp::control::SoaGearState::kGEAR_P;
	case eGearState_D:
		return eevp::control::SoaGearState::kGEAR_D;
	case eGearState_N:
		return eevp::control::SoaGearState::kGEAR_N;
	case eGearState_R:
		return eevp::control::SoaGearState::kGEAR_R;
	case eGearState_L:
		return eevp::control::SoaGearState::kGEAR_L;
	case eGearState_2:
		return eevp::control::SoaGearState::kGEAR_2;
	case eGearState_3:
		return eevp::control::SoaGearState::kGEAR_3;
	case eGearState_DS:
		return eevp::control::SoaGearState::kGEAR_DS;
	case eGearState_ManualShift:
		return eevp::control::SoaGearState::kGEAR_MS;
	case eGearState_PaddleShift:
		return eevp::control::SoaGearState::kGEAR_PS;
	case eGearState_SubcomComm:
		return eevp::control::SoaGearState::kGEAR_SUBCOM_COMM;
	case eGearState_Fault:
		return eevp::control::SoaGearState::kGEAR_FAULT;
	case eGearState_Unknown:
		return eevp::control::SoaGearState::kGEAR_UNKNOWN;
	default:
		break;
	}

	return eevp::control::SoaGearState::kGEAR_UNKNOWN;
}

GearState_e Convert_GearState_API2SOA(eevp::control::SoaGearState gear)
{
	switch (gear)
	{
	case eevp::control::SoaGearState::kGEAR_P:
		return eGearState_P;
	case eevp::control::SoaGearState::kGEAR_D:
		return eGearState_D;
	case eevp::control::SoaGearState::kGEAR_N:
		return eGearState_N;
	case eevp::control::SoaGearState::kGEAR_R:
		return eGearState_R;
	case eevp::control::SoaGearState::kGEAR_L:
		return eGearState_L;
	case eevp::control::SoaGearState::kGEAR_2:
		return eGearState_2;
	case eevp::control::SoaGearState::kGEAR_3:
		return eGearState_3;
	case eevp::control::SoaGearState::kGEAR_DS:
		return eGearState_DS;
	case eevp::control::SoaGearState::kGEAR_MS:
		return eGearState_ManualShift;
	case eevp::control::SoaGearState::kGEAR_PS:
		return eGearState_PaddleShift;
	case eevp::control::SoaGearState::kGEAR_SUBCOM_COMM:
		return eGearState_SubcomComm;
	case eevp::control::SoaGearState::kGEAR_FAULT:
		return eGearState_Fault;
	case eevp::control::SoaGearState::kGEAR_UNKNOWN:
		return eGearState_Unknown;
	default:
		break;
	}

	return eGearState_Unknown;
}

// IgnState_e
eevp::control::SoaIgnState Convert_IgnState_SOA2API(IgnState_e ign)
{
	switch (ign)
	{
	case eIgnState_Off:
		return eevp::control::SoaIgnState::kOFF;
	case eIgnState_On:
		return eevp::control::SoaIgnState::kON;
	case eIgnState_Error:
		return eevp::control::SoaIgnState::kERROR;
	default:
		break;
	}

	return eevp::control::SoaIgnState::kERROR;
}

IgnState_e Convert_IgnState_API2SOA(eevp::control::SoaIgnState ign)
{
	switch (ign)
	{
	case eevp::control::SoaIgnState::kOFF:
		return eIgnState_Off;
	case eevp::control::SoaIgnState::kON:
		return eIgnState_On;
	case eevp::control::SoaIgnState::kERROR:
		return eIgnState_Error;
	default:
		break;
	}

	return eIgnState_Error;   
}

// AccState_e
eevp::control::SoaAccState Convert_AccState_SOA2API(AccState_e acc)
{
	switch (acc)
	{
	case eAccState_Off:
		return eevp::control::SoaAccState::kOFF;
	case eAccState_On:
		return eevp::control::SoaAccState::kON;
	case eAccState_Error:
		return eevp::control::SoaAccState::kERROR;
	default:
		break;
	}

	return eevp::control::SoaAccState::kERROR;
}

AccState_e Convert_AccState_API2SOA(eevp::control::SoaAccState acc)
{
	switch (acc)
	{
	case eevp::control::SoaAccState::kOFF:
		return eAccState_Off;
	case eevp::control::SoaAccState::kON:
		return eAccState_On;
	case eevp::control::SoaAccState::kERROR:
		return eAccState_Error;
	default:
		break;
	}

	return eAccState_Error;
}
