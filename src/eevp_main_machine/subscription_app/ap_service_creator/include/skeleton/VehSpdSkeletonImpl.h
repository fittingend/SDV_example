#ifndef __VEHSPD_SKELETON_H__
#define __VEHSPD_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/vcs_vehspd_skeleton.h"
#include "listener/IVehSpdListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class VehSpdSkeletonImpl : public eevp::simulation::skeleton::VCS_VehSpdSkeleton
        {
        public:
            VehSpdSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~VehSpdSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IVehSpdListener> _listener);

            Future<skeleton::VCS_VehSpdSkeleton::notifyVehSpdOutput> notifyVehSpd();


        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::IVehSpdListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __VEHSPD_SKELETON_H__