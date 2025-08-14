#ifndef __ACCRPEDAL_SKELETON_H__
#define __ACCRPEDAL_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/vcs_accrpedal_skeleton.h"
#include "listener/IAccrPedalListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class AccrPedalSkeletonImpl : public eevp::simulation::skeleton::VCS_AccrPedalSkeleton
        {
        public:
            AccrPedalSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~AccrPedalSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IAccrPedalListener> _listener);

            Future<skeleton::VCS_AccrPedalSkeleton::notifyAccrPedalOutput> notifyAccrPedal();

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::IAccrPedalListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __ACCRPEDAL_SKELETON_H__