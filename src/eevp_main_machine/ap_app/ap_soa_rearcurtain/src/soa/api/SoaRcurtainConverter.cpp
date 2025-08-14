#include <Common.h>
#include <SoaDataType.h>
#include <SoaRcurtainConverter.hpp>

// DeviceNormal
eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(const DeviceNormal_e &isNormal)
{
	switch (isNormal)
	{
	case eDeviceNormal_Ok:
		return eevp::control::SoaDeviceIsNormal::kNORMAL;
	case eDeviceNormal_Abnormal:
		return eevp::control::SoaDeviceIsNormal::kABNORMAL;
	default:
		break;
	}

	return eevp::control::SoaDeviceIsNormal::kABNORMAL;
}

DeviceNormal_e Convert_DeviceNormal_API2SOA(const eevp::control::SoaDeviceIsNormal &isNormal)
{
	switch (isNormal)
	{
	case eevp::control::SoaDeviceIsNormal::kNORMAL:
		return eDeviceNormal_Ok;
	case eevp::control::SoaDeviceIsNormal::kABNORMAL:
		return eDeviceNormal_Abnormal;
	default:
		break;
	}

	return eDeviceNormal_Abnormal;
}

// ErrorState
eevp::control::SoaErrorState Convert_ErrorState_SOA2API(const ErrorState_e &error)
{
	switch (error)
	{
	case eERROR_STATE_Ok:
		return eevp::control::SoaErrorState::kOK;
	case eERROR_STATE_Error:
		return eevp::control::SoaErrorState::kERROR;
	default:
		break;
	}

	return eevp::control::SoaErrorState::kERROR;
}

ErrorState_e Convert_ErrorState_API2SOA(const eevp::control::SoaErrorState &error)
{
	switch (error)
	{
	case eevp::control::SoaErrorState::kOK:
		return eERROR_STATE_Ok;
	case eevp::control::SoaErrorState::kERROR:
		return eERROR_STATE_Error;
	default:
		break;
	}

	return eERROR_STATE_Error;
}

// Motor Direction
eevp::control::SoaRctnMotorDir Convert_MotorDir_SOA2API(const RCtnMotorDir_e &dir)
{
	switch (dir)
	{
	case eRCtnMotorDir_Off:
		return eevp::control::SoaRctnMotorDir::kSTOP;
	case eRCtnMotorDir_CloseOn:
		return eevp::control::SoaRctnMotorDir::kUP;
	case eRCtnMotorDir_OpenOn:
		return eevp::control::SoaRctnMotorDir::kDOWN;
	default:
		break;
	}

	return eevp::control::SoaRctnMotorDir::kSTOP;
}

RCtnMotorDir_e Convert_MotorDir_API2SOA(const eevp::control::SoaRctnMotorDir &dir)
{
	switch (dir)
	{
	case eevp::control::SoaRctnMotorDir::kSTOP:
		return eRCtnMotorDir_Off;
	case eevp::control::SoaRctnMotorDir::kUP:
		return eRCtnMotorDir_CloseOn;
	case eevp::control::SoaRctnMotorDir::kDOWN:
		return eRCtnMotorDir_OpenOn;
	default:
		break;
	}

	return eRCtnMotorDir_Off;
}

// Motor Position
uint8_t Convert_MotorPos_SOA2API(const RCtnPos_e &pos)
{
	switch (pos)
	{
	case eRCtnPos_FullyClosed:
		return 100;
	case eRCtnPos_Closed66:
		return 66;
	case eRCtnPos_Closed33:
		return 33;
	case eRCtnPos_FullyOpened:
		return 0;
	default:
		break;
	}

	return 0;
}

RCtnPos_e Convert_MotorPos_API2SOA(const uint8_t &pos)
{
	if (pos >= 83)
	{
		return eRCtnPos_FullyClosed;
	}
	else if (pos >= 49)
	{
		return eRCtnPos_Closed66;
	}
	else if (pos >= 17)
	{
		return eRCtnPos_Closed33;
	}

	return eRCtnPos_FullyOpened;
}

// State
eevp::control::SoaRctnState Convert_RctnState_SOA2API(const RCtnState_e &state)
{
	switch (state)
	{
	case eRCtnState_FullyClosed:
		return eevp::control::SoaRctnState::kFULLY_UP;
	case eRCtnState_FullyOpened:
		return eevp::control::SoaRctnState::kFULLY_DOWN;
	case eRCtnState_PartlyOpened:
		return eevp::control::SoaRctnState::kPARTLY_OPENED;
	case eRCtnState_Closing:
		return eevp::control::SoaRctnState::kMOVING_UP;
	case eRCtnState_Opening:
		return eevp::control::SoaRctnState::kMOVING_DOWN;
	default:
		break;
	}

	return eevp::control::SoaRctnState::kFULLY_UP;
}

RCtnState_e Convert_RctnState_API2SOA(const eevp::control::SoaRctnState &state)
{
	switch (state)
	{
	case eevp::control::SoaRctnState::kFULLY_UP:
		return eRCtnState_FullyClosed;
	case eevp::control::SoaRctnState::kFULLY_DOWN:
		return eRCtnState_FullyOpened;
	case eevp::control::SoaRctnState::kPARTLY_OPENED:
		return eRCtnState_PartlyOpened;
	case eevp::control::SoaRctnState::kMOVING_UP:
		return eRCtnState_Closing;
	case eevp::control::SoaRctnState::kMOVING_DOWN:
		return eRCtnState_Opening;
	default:
		break;
	}

	return eRCtnState_FullyClosed;
}

// MotorPos <-> State
RCtnState_e Convert_MotorPos2RctnState(const RCtnPos_e &pos)
{
	switch (pos)
	{
	case eRCtnPos_FullyClosed:
		return eRCtnState_FullyClosed;
	case eRCtnPos_Closed66:
		return eRCtnState_PartlyOpened;
	case eRCtnPos_Closed33:
		return eRCtnState_PartlyOpened;
	case eRCtnPos_FullyOpened:
		return eRCtnState_FullyOpened;
	default:
		break;
	}

	return eRCtnState_Unknown;
}

