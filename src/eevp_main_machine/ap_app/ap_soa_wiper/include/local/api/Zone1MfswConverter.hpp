#ifndef __ZONE1_MFSW_CONVERTER_HPP__
#define __ZONE1_MFSW_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// MfswWiperSw_e
extern std::uint8_t Convert_MfswWiperSwitch_SOA2Zone(MfswWiperSw_e swt);
extern MfswWiperSw_e Convert_MfswWiperSwitch_Zone2SOA(std::uint8_t swt);

// MfswWasherSw_e
extern std::uint8_t Convert_MfswWasherSwitch_SOA2Zone(MfswWasherSw_e swt);
extern MfswWasherSw_e Convert_MfswWasherSwitch_Zone2SOA(std::uint8_t swt);

// MfswWiperIntStepSw_e
extern std::uint8_t Convert_MfswWiperIntStep_SOA2Zone(MfswWiperIntStepSw_e swt);
extern MfswWiperIntStepSw_e Convert_MfswWiperIntStep_Zone2SOA(std::uint8_t swt);

// MfswWiperMistSw_e
extern std::uint8_t Convert_MfswMistSwitch_SOA2Zone(MfswWiperMistSw_e swt);
extern MfswWiperMistSw_e Convert_MfswMistSwitch_Zone2SOA(std::uint8_t swt);

#endif // __ZONE1_MFSW_CONVERTER_HPP__
