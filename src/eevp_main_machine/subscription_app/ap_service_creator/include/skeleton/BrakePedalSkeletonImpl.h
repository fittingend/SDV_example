#ifndef __BRAKEPEDAL_SKELETON_H__
#define __BRAKEPEDAL_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/vcs_brakepedal_skeleton.h"
#include "listener/IBrakePedalListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class BrakePedalSkeletonImpl : public eevp::simulation::skeleton::VCS_BrakePedalSkeleton
        {
        public:
            BrakePedalSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~BrakePedalSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IBrakePedalListener> _listener);

            Future<skeleton::VCS_BrakePedalSkeleton::notifyBrakeStatusOutput> notifyBrakeStatus();
            Future<skeleton::VCS_BrakePedalSkeleton::notifyBrakeSwitchOutput> notifyBrakeSwitch();


        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::IBrakePedalListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __BRAKEPEDAL_SKELETON_H__