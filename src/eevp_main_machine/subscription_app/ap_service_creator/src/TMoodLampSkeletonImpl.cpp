#include "skeleton/TMoodLampSkeletonImpl.h"

namespace eevp
{
    namespace simulation
    {
        TEevpControlSoaMImSkeletonImpl::TEevpControlSoaMImSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode) : listener{nullptr},
                                                       EevpControlSoaMImSkeleton(instanceSpec, mode),
                                                       mLogger(ara::log::CreateLogger("SMLM", "SMLM", ara::log::LogLevel::kInfo))

        {
            mLogger.LogInfo() << __func__;
        }

        TEevpControlSoaMImSkeletonImpl::~TEevpControlSoaMImSkeletonImpl()
        {
            StopOfferService();
        }

        void
        TEevpControlSoaMImSkeletonImpl::setEventListener(std::shared_ptr<ITMoodLampListener> _listener)
        {
            listener = _listener;
        }
        Future<skeleton::EevpControlSoaMImSkeleton::RequestMImSetBrightnessOutput>
        TEevpControlSoaMImSkeletonImpl::RequestMImSetBrightness(const std::uint16_t &Brightness)
        {
            // mLogger.LogInfo() << __func__;

            EevpControlSoaMImSkeleton::RequestMImSetBrightnessOutput response;
            Promise<RequestMImSetBrightnessOutput> promise;

            if (listener != nullptr)
            {
                listener->RequestMImSetBrightness(Brightness);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::EevpControlSoaMImSkeleton::RequestMImSetModeOutput>
        TEevpControlSoaMImSkeletonImpl::RequestMImSetMode(const lmp::mode::SoaMImMoodeMode &mood)

        {
            // mLogger.LogInfo() << __func__;

            EevpControlSoaMImSkeleton::RequestMImSetModeOutput response;
            Promise<RequestMImSetModeOutput> promise;

            if (listener != nullptr)
            {
                listener->RequestMImSetMode(mood);
            }

            promise.set_value(response);
            return promise.get_future();
        }

        Future<skeleton::EevpControlSoaMImSkeleton::RequestMImSetRGBColorOutput>
        TEevpControlSoaMImSkeletonImpl::RequestMImSetRGBColor(const std::uint8_t &ColorIndex)
        {
            {
                // mLogger.LogInfo() << __func__;

                EevpControlSoaMImSkeleton::RequestMImSetRGBColorOutput response;
                Promise<RequestMImSetRGBColorOutput> promise;

                if (listener != nullptr)
                {
                    listener->RequestMImSetRGBColor(ColorIndex);
                }

                promise.set_value(response);
                return promise.get_future();
            }
        }
    }
}
