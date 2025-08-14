#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>
#include <string.h>
#include <SmartFilmDataTypeConverter.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soafilmpos.h"
#include "eevp/control/impl_type_soafilmopacityarray.h"

// SmartFilm Position
eevp::control::SoaFilmPos Convert_SmtfilmPos_SOA2API(SmtfilmPos_e pos)
{
    switch (pos)
    {
    case eSmtfilmPos_WindShield:
        return eevp::control::SoaFilmPos::kWINDSHIELD;
    case eSmtfilmPos_RoofGlass:
         return eevp::control::SoaFilmPos::kROOF_GLASS;
    case eSmtfilmPos_DsFrontRear:
         return eevp::control::SoaFilmPos::kDS_FRONTREAR;
    case eSmtfilmPos_DsQuarter:
         return eevp::control::SoaFilmPos::kDS_QUARTER;
    case eSmtfilmPos_AsFrontRear:
         return eevp::control::SoaFilmPos::kAS_FRONTREAR;
    case eSmtfilmPos_AsQuarter:
         return eevp::control::SoaFilmPos::kAS_QUARTER;
    case eSmtfilmPos_RearGrass:
         return eevp::control::SoaFilmPos::kREAR_GLASS;
    default:
        break;
    }

    return eevp::control::SoaFilmPos::kREAR_GLASS;
}

SmtfilmPos_e Convert_SmtfilmPos_API2SOA(eevp::control::SoaFilmPos pos)
{
    switch (pos)
    {
    case eevp::control::SoaFilmPos::kWINDSHIELD:
        return eSmtfilmPos_WindShield;
    case eevp::control::SoaFilmPos::kROOF_GLASS:
         return eSmtfilmPos_RoofGlass;
    case eevp::control::SoaFilmPos::kDS_FRONTREAR:
         return eSmtfilmPos_DsFrontRear;
    case eevp::control::SoaFilmPos::kDS_QUARTER:
         return eSmtfilmPos_DsQuarter;
    case eevp::control::SoaFilmPos::kAS_FRONTREAR:
         return eSmtfilmPos_AsFrontRear;
    case eevp::control::SoaFilmPos::kAS_QUARTER:
         return eSmtfilmPos_AsQuarter;
    case eevp::control::SoaFilmPos::kREAR_GLASS:
         return eSmtfilmPos_RearGrass;
    default:
        break;
    }

    return eSmtfilmPos_RearGrass;
}

// SmartFilm Opacities
void Convert_SmtfilmOpacity_SOA2API(const SmtfilmOpacities_t &src, eevp::control::SoaFilmOpacityArray &dst)
{
    for (int i = 0; i < dst.size(); i++)
    {
        dst[i] = (i < eSmtfilePos_MAX) ? (std::uint8_t)src.mOpacities[i] : (std::uint8_t)0;
    }
}

void Convert_SmtfilmOpacity_API2SOA(const eevp::control::SoaFilmOpacityArray &src, SmtfilmOpacities_t &dst)
{
    for (int i = 0; i < eSmtfilePos_MAX; i++)
    {
        dst.mOpacities[i] = (i < src.size()) ? (int)src[i] : (int)0;
    }
}

bool CompareAndUpdate_SmtfilmOpacity_API(const eevp::control::SoaFilmOpacityArray &src, eevp::control::SoaFilmOpacityArray &dst, bool update)
{
    bool isUpdated = false;
    for (int i = 0; i < dst.size(); i++)
    {
        if (src[i] != dst[i])
        {
            isUpdated = true;
        }
    }

    if (update && isUpdated)
    {
        for (int i = 0; i < dst.size(); i++)
        {
            dst[i] = src[i];
        }
    }

    return isUpdated;
}

