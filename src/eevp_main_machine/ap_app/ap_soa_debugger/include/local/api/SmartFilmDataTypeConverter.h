
#ifndef __SMARTFILM_DATATYPE_CONVERTER_H__
#define __SMARTFILM_DATATYPE_CONVERTER_H__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soafilmpos.h"
#include "eevp/control/impl_type_soafilmopacityarray.h"

// SmartFilm Position
extern eevp::control::SoaFilmPos Convert_SmtfilmPos_SOA2API(SmtfilmPos_e pos);
extern SmtfilmPos_e Convert_SmtfilmPos_API2SOA(eevp::control::SoaFilmPos pos);

// SmartFilm Opacities
extern void Convert_SmtfilmOpacity_SOA2API(const SmtfilmOpacities_t &src, eevp::control::SoaFilmOpacityArray &dst);
extern void Convert_SmtfilmOpacity_API2SOA(const eevp::control::SoaFilmOpacityArray &src, SmtfilmOpacities_t &dst);
extern bool CompareAndUpdate_SmtfilmOpacity_API(const eevp::control::SoaFilmOpacityArray &src, eevp::control::SoaFilmOpacityArray &dst, bool update = true);

#endif // __SMARTFILM_DATATYPE_CONVERTER_H__