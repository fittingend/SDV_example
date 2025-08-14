#include "SoaMlmSkeletonImpl.hpp"
#include <Common.h>
#include <SoaDataType.h>
#include <ApiMoodlamp_Local.hpp>
#include <Log.hpp>


using namespace eevp::control::skeleton;

namespace eevp {
namespace control {

SoaMlmSkeletonImpl::SoaMlmSkeletonImpl(
        ara::core::InstanceSpecifier instanceSpec,
        ara::com::MethodCallProcessingMode mode) :
    SoaMlmSkeleton(instanceSpec, mode) ,
    mLogger(ara::log::CreateLogger("SOA", "MLM", ara::log::LogLevel::kInfo))
{
    auto soaMlmStatus_get_handler = [this]() {
        return this->soaMlmStatusGetter();
    };

    soaMlmStatus.RegisterGetHandler(soaMlmStatus_get_handler);
}

ara::core::Future<fields::soaMlmStatus::FieldType> SoaMlmSkeletonImpl::soaMlmStatusGetter()
{
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmStatusGetter] (+)\n";
    ara::core::Promise<fields::soaMlmStatus::FieldType> promise;
    promise.set_value(this->mField);
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::soaMlmStatusGetter] (-)\n";
    return promise.get_future();
}

SoaMlmSkeletonImpl::~SoaMlmSkeletonImpl() {
    StopOfferService();
}

void SoaMlmSkeletonImpl::RequestMlmSetBrightness(const std::uint16_t& brightness)
{
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::RequestMlmSetBrightness] (+)" << (int)brightness << "\n";
    Api_Moodlamp_Method_RequestMlmSetBrightness((int)brightness);
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::RequestMlmSetBrightness] (-)\n";
}

void SoaMlmSkeletonImpl::RequestMlmSetMode(const eevp::control::SoaMlmMoodMode& mood)
{
    MlmMoodMode_e mode;

    LOG_DEBUG() << "[SoaMlmSkeletonImpl::RequestMlmSetMode] " << (int)mood << "\n";

    switch (mood)
    {
    case eevp::control::SoaMlmMoodMode::kCARE_MOOD:
        mode = eMlmMoodMode_CareMood;
        break;

    case eevp::control::SoaMlmMoodMode::kCOMPORT_MOOD:
        mode = eMlmMoodMode_ComfortMood;
        break;

    case eevp::control::SoaMlmMoodMode::kDELIGHT_MOOD:
        mode = eMlmMoodMode_DelightMood;
        break;

    case eevp::control::SoaMlmMoodMode::kMOOD_THERAPHY:
        mode = eMlmMoodMode_MoodTherapy;
        break;

    case eevp::control::SoaMlmMoodMode::kVITALITY_MOOD:
        mode = eMlmMoodMode_VitalityMood;
        break;

    default:
        mode = eMlmMoodMode_CareMood;
        break;
    }

    Api_Moodlamp_Method_RequestMlmSetMode(mode);
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::RequestMlmSetMode] (-)\n";
}

void SoaMlmSkeletonImpl::RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex)
{
    Api_Moodlamp_Method_RequestMlmSetRgbColor((int)colorTableIndex);
}

void SoaMlmSkeletonImpl::SetSoaMlmStatus(eevp::control::SoaMlmStatus status)
{
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::SetSoaMlmStatus] (+)\n";
    if ((this->mField.colorTableIndex != status.colorTableIndex) ||
        (this->mField.moodMode != status.moodMode) ||
        (this->mField.brightness != status.brightness) ||
        (this->mField.isNormal != status.isNormal))
    {
        this->mField = status;
        soaMlmStatus.Update(this->mField);
    }
    LOG_DEBUG() << "[SoaMlmSkeletonImpl::SetSoaMlmStatus] (-)\n";
}


} // namespace control
} // namespace eevp
