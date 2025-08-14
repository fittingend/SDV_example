#ifndef __BMSINFO_SKELETON_H__
#define __BMSINFO_SKELETON_H__

#include "ara/log/logger.h"
#include "battery/bmsinfo/bmsinfosrv_skeleton.h"
#include "stdlib.h"

using namespace ara::core;

namespace eevp
{
    namespace simulation
    {
        class BmsInfoSkeletonImpl : public battery::bmsinfo::skeleton::BmsInfoSrvSkeleton
        {
        public:
            BmsInfoSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~BmsInfoSkeletonImpl();

            void sendEventBmsInfo(bms::input::InputData &info);
        private:
            ara::log::Logger &mLogger;

        };
    } // namespace simulation
} // namespace eevp

#endif // __BMSINFO_SKELETON_H__