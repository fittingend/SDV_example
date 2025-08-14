#include "skeleton/AccrPedalSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        AccrPedalSkeletonImpl::AccrPedalSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : listener{nullptr},
                                                       VCS_AccrPedalSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("ACPD", "ACPD", ara::log::LogLevel::kInfo))

        {
            mLogger.LogInfo() << __func__;
        }

        AccrPedalSkeletonImpl::~AccrPedalSkeletonImpl()
        {
            StopOfferService();
        }

        void
        AccrPedalSkeletonImpl::setEventListener(std::shared_ptr<IAccrPedalListener> _listener)
        {
            listener = _listener;
        }

        Future<skeleton::VCS_AccrPedalSkeleton::notifyAccrPedalOutput>
        AccrPedalSkeletonImpl::notifyAccrPedal()
        {
            // mLogger.LogInfo() << __func__;

            skeleton::VCS_AccrPedalSkeleton::notifyAccrPedalOutput response;
            Promise<notifyAccrPedalOutput> promise;

            if (listener != nullptr)
            {
                listener->notifyAccrPedal(response.VCS_AccrPedal);
            }

            promise.set_value(response);
            return promise.get_future();
        }
    }
}
