#include <Common.h>
#include <SoaDataType.h>
#include <Zone1MfswConverter.hpp>

// MfswWiperSw_e
std::uint8_t Convert_MfswWiperSwitch_SOA2Zone(MfswWiperSw_e swt)
{
	switch (swt)
	{
	case eMfswWiperSw_Off:
		return 0x00;
	case eMfswWiperSw_Int:
		return 0x01;
	case eMfswWiperSw_Auto:
		return 0x01;
	case eMfswWiperSw_Low:
		return 0x02;
	case eMfswWiperSw_High:
		return 0x03;
	default:
		break;
	}

	return 0x00;
}

MfswWiperSw_e Convert_MfswWiperSwitch_Zone2SOA(std::uint8_t swt)
{
	switch (swt)
	{
	case 0x00:
		return eMfswWiperSw_Off;
	case 0x01:
		return eMfswWiperSw_Int;
		// return eMfswWiperSw_Auto;
	case 0x02:
		return eMfswWiperSw_Low;
	case 0x03:
		return eMfswWiperSw_High;
	default:
		break;
	}

	return eMfswWiperSw_Unknown;
}

// MfswWasherSw_e
std::uint8_t Convert_MfswWasherSwitch_SOA2Zone(MfswWasherSw_e swt)
{
	switch (swt)
	{
	case eMfswWasherSw_Off:
		return 0x00;
	case eMfswWasherSw_On:
		return 0x01;
	default:
		break;
	}

	return 0x03;
}

MfswWasherSw_e Convert_MfswWasherSwitch_Zone2SOA(std::uint8_t swt)
{
	switch (swt)
	{
	case 0x00:
		return eMfswWasherSw_Off;
	case 0x01:
		return eMfswWasherSw_On;
	case 0x03:
		return eMfswWasherSw_Error;
	default:
		break;
	}

	return eMfswWasherSw_Unknown;
}

// MfswWiperIntStepSw_e
std::uint8_t Convert_MfswWiperIntStep_SOA2Zone(MfswWiperIntStepSw_e swt)
{
	switch (swt)
	{
	case eMfswWiperIntStepSw_1:
		return 0x00;
	case eMfswWiperIntStepSw_2:
		return 0x01;
	case eMfswWiperIntStepSw_3:
		return 0x02;
	case eMfswWiperIntStepSw_4:
		return 0x03;
	case eMfswWiperIntStepSw_5:
		return 0x04;
	default:
		break;
	}

	return 0x03;
}

MfswWiperIntStepSw_e Convert_MfswWiperIntStep_Zone2SOA(std::uint8_t swt)
{
	switch (swt)
	{
	case 0x00:
		return eMfswWiperIntStepSw_1;
	case 0x01:
		return eMfswWiperIntStepSw_2;
	case 0x02:
		return eMfswWiperIntStepSw_3;
	case 0x03:
		return eMfswWiperIntStepSw_4;
	case 0x04:
		return eMfswWiperIntStepSw_5;
	default:
		break;
	}

	return eMfswWiperIntStepSw_Unknown;
}

// MfswWiperMistSw_e
std::uint8_t Convert_MfswMistSwitch_SOA2Zone(MfswWiperMistSw_e swt)
{
	switch (swt)
	{
	case eMfswWiperMistSw_Off:
		return 0x00;
	case eMfswWiperMistSw_On:
		return 0x01;
	default:
		break;
	}

	return 0x00;
}

MfswWiperMistSw_e Convert_MfswMistSwitch_Zone2SOA(std::uint8_t swt)
{
	switch (swt)
	{
	case 0x00:
		return eMfswWiperMistSw_Off;
	case 0x01:
		return eMfswWiperMistSw_On;
	default:
		break;
	}

	return eMfswWiperMistSw_Unknown;
}