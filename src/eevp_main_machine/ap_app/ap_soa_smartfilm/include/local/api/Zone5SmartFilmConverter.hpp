#ifndef __ZONE5_SMARTFILM_CONVERTER_HPP__
#define __ZONE5_SMARTFILM_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>
#include "eevp/control/impl_type_soafilmopacityarray.h"

// DeviceNormal
extern uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(uint8_t isNormal);

// SmartFilm Position
extern uint8_t Convert_SmtfilmPos_SOA2Zone(SmtfilmPos_e pos);
extern SmtfilmPos_e Convert_SmtfilmPos_Zone2SOA(uint8_t pos);

// SmartFilm Opacities
extern void Convert_SmtfilmOpacity_SOA2Zone(const SmtfilmOpacities_t &src, eevp::control::SoaFilmOpacityArray &dst);
extern void Convert_SmtfilmOpacity_Zone2SOA(const eevp::control::SoaFilmOpacityArray &src, SmtfilmOpacities_t &dst);

#endif // __ZONE5_SMARTFILM_CONVERTER_HPP__