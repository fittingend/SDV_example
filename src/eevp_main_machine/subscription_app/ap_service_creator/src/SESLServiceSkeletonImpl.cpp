#include "skeleton/SESLServiceSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        SESLServiceSkeletonImpl::SESLServiceSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : listener{nullptr},
                                                       SESL_ServiceInterfaceSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("SESL", "SESL", ara::log::LogLevel::kInfo))

        {
            mLogger.LogInfo() << __func__;
        }

        SESLServiceSkeletonImpl::~SESLServiceSkeletonImpl()
        {
            StopOfferService();
        }

        void
        SESLServiceSkeletonImpl::setEventListener(std::shared_ptr<eevp::simulation::ISESLServiceListener> _listener)
        {
            listener = _listener;
        }

        Future<ara::com::skeleton::SESL_ServiceInterfaceSkeleton::SESL_ReceiveOutput>
        SESLServiceSkeletonImpl::SESL_Receive()
        {
            // mLogger.LogInfo() << __func__;

            ara::com::skeleton::SESL_ServiceInterfaceSkeleton::SESL_ReceiveOutput response;
            Promise<SESL_ReceiveOutput> promise;

            if (listener != nullptr)
            {
                listener->SESL_Receive(response.Receive_Argument);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        void
        SESLServiceSkeletonImpl::SESL_Send(const ara::SESL::Vehicle_Data &Send_Argument)
        {
            // mLogger.LogInfo() << __func__;
            if (listener != nullptr)
            {
                listener->SESL_Send(Send_Argument);
            }
            return;
        }
    }
}
