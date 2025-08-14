#ifndef __WIPER_SKELETON_H__
#define __WIPER_SKELETON_H__

#include "ara/log/logger.h"
#include "eevp/simulation/bcm_wiperwash_skeleton.h"
#include "stdlib.h"


#include "listener/IWiperListener.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class WiperSkeletonImpl : public eevp::simulation::skeleton::BCM_WiperWashSkeleton
        {
        public:
            WiperSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~WiperSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::IWiperListener> _listener);
            Future<stopWipingOutput> stopWiping();

            Future<startWipingOutput> startWiping();

            Future<setWipingLevelOutput> setWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel);
            Future<setWipingIntervalOutput> setWipingInterval(const std::uint16_t &wipingInterval);

            Future<simulation::skeleton::fields::wiperLevel::FieldType> wiperLevelGetter();
            void updateWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel);

            Future<simulation::skeleton::fields::wiperInterval::FieldType> wiperIntervalGetter();
            void updateWipingInterval(const std::uint16_t &wipingInterval);

        private:
            ara::log::Logger &mLogger;

            bool updateWiperLevelContext();

            std::shared_ptr<eevp::simulation::IWiperListener> listener;

            simulation::skeleton::fields::wiperLevel::FieldType mWiperLevel;
            simulation::skeleton::fields::wiperInterval::FieldType mWiperInterval;
        };
    } // namespace service
} // namespace eevp

#endif // __WIPER_SKELETON_H__