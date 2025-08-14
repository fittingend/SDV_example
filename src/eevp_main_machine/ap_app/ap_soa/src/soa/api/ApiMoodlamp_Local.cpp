#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <VehicleContext.hpp>
#include <MoodLampManager.hpp>
#include <ApiMoodlamp_Local.hpp>
#include <SoaMlmSkeletonImpl.hpp>
#if !defined(DISABLE_SOMEIP)
#include <PaconSetting.hpp>
#endif
#include <Log.hpp>

void Api_Moodlamp_Method_RequestMlmSetRgbColor(int index)
{
    MoodLampManager *mgr = MoodLampManager::GetInstance();
    mgr->OnCmdRequestMlmSetRgbColor(index);
}

void Api_Moodlamp_Method_RequestMlmSetMode(MlmMoodMode_e mode)
{
    MoodLampManager *mgr = MoodLampManager::GetInstance();
    mgr->OnCmdRequestMlmSetMode(mode);
}

void Api_Moodlamp_Method_RequestMlmSetBrightness(int brightness)
{
    MoodLampManager *mgr = MoodLampManager::GetInstance();
    mgr->OnCmdRequestMlmSetBrightness(brightness);
}

void Api_Moodlamp_Field_SoaMlmStatus()
{
    eevp::control::SoaMlmStatus mlmStatus;
    VehicleContext *context = VehicleContext::GetInstance();

    std::uint8_t colorTableIndex;
    eevp::control::SoaMlmMoodMode moodMode;
    std::uint16_t brightness;
    eevp::control::SoaDeviceIsNormal isNormal;

    colorTableIndex = (std::uint8_t)context->mMlmColorIndex;

    switch (context->mMlmMoodMode)
    {
    case eMlmMoodMode_CareMood:
        moodMode = eevp::control::SoaMlmMoodMode::kCARE_MOOD;
        break;
    case eMlmMoodMode_ComfortMood:
        moodMode = eevp::control::SoaMlmMoodMode::kCOMPORT_MOOD;
        break;
    case eMlmMoodMode_DelightMood:
        moodMode = eevp::control::SoaMlmMoodMode::kDELIGHT_MOOD;
        break;
    case eMlmMoodMode_MoodTherapy:
        moodMode = eevp::control::SoaMlmMoodMode::kMOOD_THERAPHY;
        break;
    case eMlmMoodMode_VitalityMood:
        moodMode = eevp::control::SoaMlmMoodMode::kVITALITY_MOOD;
        break;
    default:
        moodMode = eevp::control::SoaMlmMoodMode::kCARE_MOOD;
        break;
    }

    brightness = (std::uint16_t)context->mMlmBrightness;

    isNormal = eevp::control::SoaDeviceIsNormal::kNORMAL;


    mlmStatus.colorTableIndex = colorTableIndex;
    mlmStatus.moodMode = moodMode;
    mlmStatus.brightness = brightness;
    mlmStatus.isNormal = isNormal;

#if !defined(DISABLE_SOMEIP)
    PaconSetting *pacon = PaconSetting::GetInstance();
    if (pacon != nullptr)
    {
        eevp::control::SoaMlmSkeletonImpl *mlm = pacon->GetSoaMlm();
        if (mlm != nullptr)
        {
            mlm->SetSoaMlmStatus(mlmStatus);
        }
    }
#else
    LOG_DEBUG() << "[Api_Moodlamp_Field_SoaMlmStatus] mlmStatus.colorTableIndex = " << (int)mlmStatus.colorTableIndex << "\n";
    LOG_DEBUG() << "[Api_Moodlamp_Field_SoaMlmStatus] mlmStatus.moodMode = " << (int)mlmStatus.moodMode << "\n";
    LOG_DEBUG() << "[Api_Moodlamp_Field_SoaMlmStatus] mlmStatus.brightness = " << (int)mlmStatus.brightness << "\n";
    LOG_DEBUG() << "[Api_Moodlamp_Field_SoaMlmStatus] mlmStatus.isNormal = " << (int)mlmStatus.isNormal << "\n";
#endif
}
