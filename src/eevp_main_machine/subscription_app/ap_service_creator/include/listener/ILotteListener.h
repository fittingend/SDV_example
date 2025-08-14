#ifndef __ILOTTE_LISTENER_H__
#define __ILOTTE_LISTENER_H__

#include "lotte/type/impl_type_dmsgzdtctn.h"
#include "lotte/type/impl_type_gazazone.h"
#include "lotte/type/impl_type_headzone.h"

namespace eevp
{
    namespace simulation
    {
        class ILotteListener
        {
        public:
            virtual ~ILotteListener() {};

            virtual void NotifyDmsCurr(lotte::type::DmsGzDtctn &dmsGzDtctn) = 0;
            virtual void NotifyDmsStatus(bool &result) = 0;
            virtual void NotifySmartFilmCurr(lotte::type::SideTransparency &result) = 0;
            virtual void NotifySmartFilmStatus(bool &result) = 0;
            virtual void SmartFilmControl(const std::int32_t &windowLoc, const std::int32_t &transparence) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __ILOTTE_LISTENER_H__