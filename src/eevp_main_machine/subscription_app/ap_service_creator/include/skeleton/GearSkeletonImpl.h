#ifndef __GEAR_SKELETON_H__
#define __GEAR_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/vcs_gear_skeleton.h"
#include "listener/IGearListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class GearSkeletonImpl : public eevp::simulation::skeleton::VCS_GearSkeleton
        {
        public:
            GearSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~GearSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IGearListener> _listener);

            Future<skeleton::VCS_GearSkeleton::notifyGearOutput> notifyGear();
            Future<setTargetOutput> setTarget(const eevp::simulation::type::VCS_Gear& targetGear);

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::IGearListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __GEAR_SKELETON_H__