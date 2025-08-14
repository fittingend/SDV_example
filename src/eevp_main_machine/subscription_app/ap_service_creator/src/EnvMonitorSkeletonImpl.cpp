#include "skeleton/EnvMonitorSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        EnvMonitorSkeletonImpl::EnvMonitorSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : listener{nullptr},
                                                       TMS_EnvMonitorSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("ENMN", "ENMN", ara::log::LogLevel::kInfo))

        {
            mLogger.LogInfo() << __func__;
        }

        EnvMonitorSkeletonImpl::~EnvMonitorSkeletonImpl()
        {
            StopOfferService();
        }

        void
        EnvMonitorSkeletonImpl::setEventListener(std::shared_ptr<IEnvMonitorListener> _listener)
        {
            listener = _listener;
        }

        Future<skeleton::TMS_EnvMonitorSkeleton::notifyHumidityOutput>
        EnvMonitorSkeletonImpl::notifyHumidity()
        {
            mLogger.LogInfo() << __func__;

            skeleton::TMS_EnvMonitorSkeleton::notifyHumidityOutput response;
            Promise<notifyHumidityOutput> promise;

            if (listener != nullptr)
            {
                listener->notifyHumidity(response.TMS_EnvZoneHumidityAry);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::TMS_EnvMonitorSkeleton::getHumidityOutput>
        EnvMonitorSkeletonImpl::getHumidity(const eevp::simulation::type::TMS_EnvZone &zoneId)
        {
            mLogger.LogInfo() << __func__;
        }
        Future<skeleton::TMS_EnvMonitorSkeleton::getLightIntensityOutput>
        EnvMonitorSkeletonImpl::getLightIntensity(const eevp::simulation::type::TMS_EnvZone &zoneId)
        {
            mLogger.LogInfo() << __func__;
        }
        Future<skeleton::TMS_EnvMonitorSkeleton::getTempOutput>
        EnvMonitorSkeletonImpl::getTemp(const eevp::simulation::type::TMS_EnvZone &zoneId)
        {
            mLogger.LogInfo() << __func__;
        }
        Future<skeleton::TMS_EnvMonitorSkeleton::notifyLightIntensityOutput>
        EnvMonitorSkeletonImpl::notifyLightIntensity()
        {
            mLogger.LogInfo() << __func__;
        }
        Future<skeleton::TMS_EnvMonitorSkeleton::notifyTempOutput>
        EnvMonitorSkeletonImpl::notifyTemp()
        {
            mLogger.LogInfo() << __func__;
        }
    }
}
