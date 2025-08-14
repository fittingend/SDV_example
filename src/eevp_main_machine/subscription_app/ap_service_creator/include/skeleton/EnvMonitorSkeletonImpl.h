#ifndef __ENVMONITOR_SKELETON_H__
#define __ENVMONITOR_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/tms_envmonitor_skeleton.h"
#include "listener/IEnvMonitorListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class EnvMonitorSkeletonImpl : public eevp::simulation::skeleton::TMS_EnvMonitorSkeleton
        {
        public:
            EnvMonitorSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~EnvMonitorSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IEnvMonitorListener> _listener);

            Future<skeleton::TMS_EnvMonitorSkeleton::notifyHumidityOutput> notifyHumidity();
            Future<getHumidityOutput> getHumidity(const eevp::simulation::type::TMS_EnvZone &zoneId);
            Future<getLightIntensityOutput> getLightIntensity(const eevp::simulation::type::TMS_EnvZone &zoneId);
            Future<getTempOutput> getTemp(const eevp::simulation::type::TMS_EnvZone &zoneId);
            Future<notifyLightIntensityOutput> notifyLightIntensity();
            Future<notifyTempOutput> notifyTemp();

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::IEnvMonitorListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __ENVMONITOR_SKELETON_H__