#ifndef __LOTTE_SKELETON_H__
#define __LOTTE_SKELETON_H__

#include "ara/log/logger.h"
#include "lotte/service/lotteservice_skeleton.h"
#include "listener/ILotteListener.h"
#include "stdlib.h"

using namespace ara::core;
using namespace lotte::service;
namespace eevp
{
    namespace simulation
    {
        class LotteSkeletonImpl : public lotte::service::skeleton::LotteServiceSkeleton
        {
        public:
            LotteSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~LotteSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::ILotteListener> _listener);

            Future<DmsCurrOutput> DmsCurr();
            Future<DmsStatusOutput> DmsStatus();
            Future<SmartFilmControlOutput> SmartFilmControl(const std::uint8_t &windowLoc, const std::uint8_t &transparence);
            Future<SmartFilmCurrOutput> SmartFilmCurr(const std::uint8_t &windowLoc);
            Future<SmartFilmStatusOutput> SmartFilmStatus();

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::ILotteListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __LOTTE_SKELETON_H__