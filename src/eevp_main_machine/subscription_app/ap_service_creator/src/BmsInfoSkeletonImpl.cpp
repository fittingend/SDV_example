#include "skeleton/BmsInfoSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        BmsInfoSkeletonImpl::BmsInfoSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : BmsInfoSrvSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("BMSI", "BMSI", ara::log::LogLevel::kInfo))
        {
            mLogger.LogInfo() << __func__;
        }

        BmsInfoSkeletonImpl::~BmsInfoSkeletonImpl()
        {
            StopOfferService();
        }

        void BmsInfoSkeletonImpl::sendEventBmsInfo(bms::input::InputData &info)
        {
            // mLogger.LogInfo() << __func__;
            ems_BmsInfo.Send(info);
        }

    }
}
