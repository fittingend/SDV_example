#ifndef __MOODLAMP_SKELETON_H__
#define __MOODLAMP_SKELETON_H__

#include "ara/log/logger.h"
#include "lmp/control/eevpcontrolsoamim_skeleton.h"
#include "stdlib.h"

#include "listener/ITMoodLampListener.h"

using namespace lmp::control;
using namespace lmp::mode;
using namespace ara::core;
using namespace eevp::simulation;

namespace eevp
{
    namespace simulation
    {
        class TEevpControlSoaMImSkeletonImpl : public lmp::control::skeleton::EevpControlSoaMImSkeleton
        {
        public:
            TEevpControlSoaMImSkeletonImpl(
                InstanceSpecifier instanceSpec,
                ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

            ~TEevpControlSoaMImSkeletonImpl();

            void setEventListener(std::shared_ptr<eevp::simulation::ITMoodLampListener> _listener);

            Future<RequestMImSetBrightnessOutput> RequestMImSetBrightness(const std::uint16_t &Brightness);
            Future<RequestMImSetModeOutput> RequestMImSetMode(const lmp::mode::SoaMImMoodeMode &mood);
            Future<RequestMImSetRGBColorOutput> RequestMImSetRGBColor(const std::uint8_t &ColorIndex);

        private:
            ara::log::Logger &mLogger;

            std::shared_ptr<eevp::simulation::ITMoodLampListener> listener;
        };
    } // namespace simulation
} // namespace eevp

#endif // __MOODLAMP_SKELETON_H__