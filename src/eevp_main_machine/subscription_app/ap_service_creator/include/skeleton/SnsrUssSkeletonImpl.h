#ifndef __SNSRUSS_SKELETON_H__
#define __SNSRUSS_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/snsr_uss_skeleton.h"
#include "listener/ISnsrUssListener.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class SnsrUssSkeletonImpl : public eevp::simulation::skeleton::Snsr_USSSkeleton
        {
        public:
            SnsrUssSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~SnsrUssSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::ISnsrUssListener> _listener);

            Future<skeleton::Snsr_USSSkeleton::ntfSonarInfoOutput> ntfSonarInfo();
            Future<skeleton::Snsr_USSSkeleton::ntfFltStOutput> ntfFltSt();
            Future<skeleton::Snsr_USSSkeleton::isDetectOutput> isDetect();

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::ISnsrUssListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __SNSRUSS_SKELETON_H__