#ifndef __SESLSERVICE_SKELETON_H__
#define __SESLSERVICE_SKELETON_H__

#include "ara/log/logger.h"
#include "ara/com/sesl_serviceinterface_skeleton.h"
#include "listener/ISESLServiceListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class SESLServiceSkeletonImpl : public ara::com::skeleton::SESL_ServiceInterfaceSkeleton
        {
        public:

            SESLServiceSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~SESLServiceSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::ISESLServiceListener> _listener);

            Future<ara::com::skeleton::SESL_ServiceInterfaceSkeleton::SESL_ReceiveOutput> SESL_Receive();
            void SESL_Send(const ara::SESL::Vehicle_Data& Send_Argument);

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::ISESLServiceListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __SESLSERVICE_SKELETON_H__