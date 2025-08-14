#include <Common.h>
#include <csignal>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <Zone1Api_Local.hpp>
#include <ApiMoodlamp_Local.hpp>
#include <MoodLampManager.hpp>

MoodLampManager *MoodLampManager::mInst = nullptr;

MoodLampManager *MoodLampManager::GetInstance()
{
    if (MoodLampManager::mInst == nullptr)
    {
        MoodLampManager::mInst = new MoodLampManager();
        Api_Moodlamp_Field_SoaMlmStatus();
    }

    return MoodLampManager::mInst;
}

MoodLampManager::MoodLampManager()
{

}

MoodLampManager::~MoodLampManager()
{

}

int MoodLampManager::OnCmdRequestMlmSetRgbColor(int colorTableIndex, bool api_modal)
{
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
    VehicleContext *context = VehicleContext::GetInstance();
    if (context->mMlmColorIndex != colorTableIndex)
    {
        context->mMlmColorIndex = colorTableIndex;
        Zone1_MoodLamp_SetColorIndex(colorTableIndex);
        Api_Moodlamp_Field_SoaMlmStatus();
    }
#else
    Zone1_MoodLamp_SetColorIndex(colorTableIndex);
#endif
    return 0;
}

int MoodLampManager::OnCmdRequestMlmSetMode(MlmMoodMode_e mode, bool api_modal)
{
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
    VehicleContext *context = VehicleContext::GetInstance();
    if (context->mMlmMoodMode != mode)
    {
        context->mMlmMoodMode = mode;
        Zone1_MoodLamp_SetColorMode(mode);
        Api_Moodlamp_Field_SoaMlmStatus();
    }
#else
    Zone1_MoodLamp_SetColorMode(mode);
#endif
    return 0;
}

int MoodLampManager::OnCmdRequestMlmSetBrightness(int brightness, bool api_modal)
{
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
    VehicleContext *context = VehicleContext::GetInstance();
    if (context->mMlmBrightness != brightness)
    {
        context->mMlmBrightness = brightness;
        Zone1_MoodLamp_SetBrightness(brightness);
        Api_Moodlamp_Field_SoaMlmStatus();
    }
#else
    Zone1_MoodLamp_SetBrightness(brightness);
#endif
    return 0;
}

